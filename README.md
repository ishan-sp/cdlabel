# Numerical Sanitizer (NumSan)

NumSan is an LLVM-based numerical sanitizer that instruments floating-point operations and reports precision loss by comparing float results against double-precision shadows. It includes a CLI wrapper for compilation/instrumentation and a React + FastAPI UI for interactive analysis.

## Features

- LLVM pass that instruments float `+`, `-`, `*`, and `/`
- Runtime checker with relative error threshold
- Line-numbered warnings (when debug info is present)
- One-command CLI pipeline via `bin/numsan`
- Web UI with editor, warnings list, and output panel

## Prerequisites

- Linux (tested on Ubuntu/WSL)
- LLVM/Clang build (matching the pass): `opt` and `clang` binaries
- CMake 3.10+
- C/C++ compiler toolchain
- Python 3.8+ (for FastAPI backend)
- Node.js 18+ and npm (for UI)

## LLVM Build (Example)

If you do not already have a local LLVM build, the following example produces the
`clang` and `opt` binaries needed by `bin/numsan`:

```
cd ~/llvm-project
cmake -S llvm -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
ninja -C build clang opt
```

## Build

Build the LLVM pass:

```
cd /path/to/cdlabel
cmake -S . -B build
cmake --build build
```

## CLI Usage

Run the sanitizer on a test file:

```
cd /path/to/cdlabel
./bin/numsan tests/test_multiply.c -o artifacts/test_multiply_san
./artifacts/test_multiply_san
```

## Web UI

Start the backend:

```
cd /path/to/cdlabel/ui/backend
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
uvicorn main:app --reload --port 8000
```

Start the UI:

```
cd /path/to/cdlabel/ui
npm install
npm run dev
```

Then open:

```
http://localhost:5173
```

## Project Structure

```
.
├── bin/                         # CLI wrapper entry point
│   └── numsan                   # Compile, instrument, link, run
├── build/                       # CMake build output
│   └── NumericalPass.so          # LLVM pass shared object
├── src/                         # Core LLVM pass + runtime
│   ├── NumericalPass.cpp         # Instrumentation pass
│   └── numsan_rt.c               # Runtime check implementation
├── tests/                       # Real-world and synthetic test programs
│   ├── test_combined.c
│   ├── test_multiply.c
│   ├── test_quadratic.c
│   ├── test_path_sum.c
│   └── ...
├── artifacts/                   # Instrumented binaries (output)
│   └── *_san                     # Executables built by numsan
├── ui/                          # Web UI + backend API
│   ├── index.html                # Vite entry HTML
│   ├── package.json              # UI dependencies/scripts
│   ├── vite.config.js            # Dev server + proxy config
│   ├── src/
│   │   ├── App.jsx               # UI logic
│   │   ├── main.jsx              # React bootstrap
│   │   └── styles.css            # Styling
│   └── backend/
│       ├── main.py               # FastAPI backend
│       ├── requirements.txt      # Backend deps
│       └── README.md             # Backend notes
├── DESIGN.md                     # Design rationale
├── IMPLEMENTATION.md             # Implementation details
├── EVALUATION.md                 # Test results
└── CMakeLists.txt                # Build configuration
```

## Decision Logic

### Instrumentation
- Only float operations (`FAdd`, `FSub`, `FMul`, `FDiv`) are instrumented.
- Each float op is recomputed in double precision to form a shadow result.
- The pass emits `__numsan_check(float actual, double shadow, int line)` calls.

### Runtime Checks
- Relative error is computed as:
  - $\text{rel} = \frac{|actual - shadow|}{|shadow|}$ when $|shadow| > 1e-10$
  - else $\text{rel} = |actual - shadow|$
- Warnings are emitted when $\text{rel} > 1e-9$.

### Build Pipeline
- `bin/numsan` compiles to LLVM IR, runs the pass, and links the runtime.
- `-g` is enabled to keep line numbers.
- `-O0` is used to avoid constant folding that would remove target ops.

## Notes and Limitations

- Only float ops are tracked (not double or vector types).
- Debug info is required for line numbers in warnings.
- High warning counts can occur in tight loops; see `tests/test_path_sum.c`.

## Documents

- [DESIGN.md](DESIGN.md) for design decisions
- [IMPLEMENTATION.md](IMPLEMENTATION.md) for implementation details
- [EVALUATION.md](EVALUATION.md) for test results
