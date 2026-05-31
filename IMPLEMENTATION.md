# IMPLEMENTATION

## LLVM and Clang Integration

The LLVM pass lives in `src/NumericalPass.cpp` and is built as `build/NumericalPass.so` by `CMakeLists.txt`. The pass is loaded by `opt` from the `bin/numsan` wrapper:

```
opt -load-pass-plugin build/NumericalPass.so -passes="function(numsan)" ...
```

The wrapper compiles source to LLVM IR, runs the pass, and links the runtime:

```
clang -S -emit-llvm -O0 -g source.c -o tmp.ll
opt -load-pass-plugin build/NumericalPass.so -passes="function(numsan)" tmp.ll -o tmp.bc
clang tmp.bc src/numsan_rt.c -o a.out -lm
```

The `-g` flag enables line numbers for UI reporting, and `-O0` keeps floating-point ops from being constant-folded away.

---

## IR Instrumentation Pipeline

`NumericalPass` scans each function for floating-point ops:

- `FAdd`
- `FSub`
- `FMul`
- `FDiv`

Only float-typed results are instrumented (not double or vector types). Each matched instruction is followed by:

1. Extend operands to double
2. Recompute the operation in double precision
3. Call `__numsan_check(float actual, double shadow, int line)`

The line number is taken from debug metadata when present, otherwise `0`.

---

## Runtime Checking

The runtime lives in `src/numsan_rt.c` and provides:

```
void __numsan_check(float actual, double shadow, int line)
```

It computes a relative error and prints a warning if it exceeds `1e-9`. Warnings include:

- Line number (or `?` if unavailable)
- Float result
- Double shadow
- Relative difference

This text format is intentionally easy to parse by the backend and readable in the CLI.

---

## Wrapper and Build Flow

`bin/numsan` is the primary entry point. It handles:

- Compilation to IR (`clang -S -emit-llvm`)
- Instrumentation via `opt`
- Linking the runtime
- Emitting a runnable binary

The wrapper is used both from the CLI and the FastAPI backend.

---

## FastAPI Backend

`ui/backend/main.py` exposes a single endpoint:

- `POST /api/numsan`

The backend:

1. Writes submitted C code to a temporary file
2. Runs `bin/numsan` to compile + instrument
3. Executes the binary
4. Parses warnings from stdout
5. Returns JSON:

```
{
  "warnings": [{ "line": 12, "message": "...", "detail": "..." }],
  "output": "raw program output"
}
```

Warnings are extracted from lines starting with `[NUMSAN] WARNING line ...`.

---

## UI Integration

The React UI lives in `ui/src/App.jsx` and posts code to `/api/numsan`. It displays:

- Editor with line numbers
- Warning cards that jump to the line
- Program output panel

`ui/vite.config.js` proxies `/api` to the backend at `http://localhost:8000` for local development.

---

## Tests

All test programs live in `tests/` and can be executed with:

```
./bin/numsan tests/test_*.c -o artifacts/<name>_san
./artifacts/<name>_san
```

`EVALUATION.md` documents the latest run results across all tests.
