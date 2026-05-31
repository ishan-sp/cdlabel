from __future__ import annotations

import re
import subprocess
import tempfile
from pathlib import Path

from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

ROOT = Path(__file__).resolve().parents[2]
NUMSAN = ROOT / "bin" / "numsan"

app = FastAPI()
app.add_middleware(
    CORSMiddleware,
    allow_origins=["http://localhost:5173"],
    allow_methods=["*"],
    allow_headers=["*"],
)


class CodePayload(BaseModel):
    code: str


def parse_warnings(output: str) -> list[dict]:
    warnings: list[dict] = []
    warning_re = re.compile(r"^\[NUMSAN\] WARNING line (?P<line>\d+|\?): (?P<msg>.+)$")
    current: dict | None = None

    for line in output.splitlines():
        match = warning_re.match(line)
        if match:
            if current:
                warnings.append(current)
            line_token = match.group("line")
            line_num = int(line_token) if line_token.isdigit() else 0
            current = {"line": line_num, "message": match.group("msg"), "detail": ""}
            continue

        if current and line.startswith("         "):
            detail = line.strip()
            current["detail"] = (
                f"{current['detail']} | {detail}" if current["detail"] else detail
            )
            continue

        if current:
            warnings.append(current)
            current = None

    if current:
        warnings.append(current)

    return warnings


@app.post("/api/numsan")
def run_numsan(payload: CodePayload) -> dict:
    if not payload.code.strip():
        raise HTTPException(status_code=400, detail="Code is empty.")
    if not NUMSAN.exists():
        raise HTTPException(status_code=500, detail="numsan wrapper not found.")

    with tempfile.TemporaryDirectory() as tmpdir:
        src_path = Path(tmpdir) / "input.c"
        bin_path = Path(tmpdir) / "a.out"
        src_path.write_text(payload.code)

        compile_result = subprocess.run(
            [str(NUMSAN), str(src_path), "-o", str(bin_path)],
            cwd=ROOT,
            capture_output=True,
            text=True,
        )

        if compile_result.returncode != 0:
            msg = f"{compile_result.stdout}\n{compile_result.stderr}".strip()
            raise HTTPException(status_code=400, detail=msg or "Compilation failed.")

        run_result = subprocess.run(
            [str(bin_path)],
            cwd=ROOT,
            capture_output=True,
            text=True,
        )

        if run_result.returncode != 0:
            msg = f"{run_result.stdout}\n{run_result.stderr}".strip()
            raise HTTPException(status_code=400, detail=msg or "Execution failed.")

        warnings = parse_warnings(run_result.stdout)
        output = run_result.stdout.strip()
        if run_result.stderr.strip():
            output = f"{output}\n{run_result.stderr.strip()}".strip()

        return {"warnings": warnings, "output": output}
