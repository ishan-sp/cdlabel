# Evaluation Report

This report summarizes running numsan across all programs in tests/. It includes warning counts, sample diagnostics, and program output snippets.

## Methodology

- Each test is compiled and instrumented with `bin/numsan`.
- The resulting binary is executed once per test.
- Warnings are emitted when relative error exceeds the runtime threshold.

Severity labels:

- None: 0 warnings
- Low: 1 to 5 warnings
- High: more than 100 warnings

## Summary

| Test | Status | Warnings | Severity | Notes |
| --- | --- | --- | --- | --- |
| test_combined | ok | 1 | Low | Loss of significance in large + small add |
| test_energy_subtract | ok | 0 | None | No warnings emitted |
| test_exchange_rate | ok | 1 | Low | Precision loss on large multiply |
| test_finance_fee | ok | 0 | None | No warnings emitted |
| test_interest_compound | ok | 19750 | High | Repeated compounding amplifies error |
| test_multiply | ok | 2 | Low | Large-scale multiply precision loss |
| test_parallel_resistance | ok | 2 | Low | Division of highly imbalanced values |
| test_path_sum | ok | 999998 | High | Accumulation drift in long loop |
| test_physics_scale | ok | 1 | Low | Large value scaled by near-1 factor |
| test_projectile | ok | 1 | Low | Small relative error in kinematics formula |
| test_quadratic | ok | 1 | Low | Cancellation in quadratic formula |
| test_sensor_highpass | ok | 0 | None | No warnings emitted |
| test_signal_cancel | ok | 0 | None | No warnings emitted |
| test_temperature_delta | ok | 0 | None | No warnings emitted |
| test_vector_norm | ok | 0 | None | Output overflow to inf without warning |

## Key Findings

- Extremely high warning rates in `test_path_sum` and `test_interest_compound` indicate cumulative precision loss in iterative updates.
- `test_vector_norm` reports `inf` without warnings, showing overflow can occur even when the relative-difference heuristic does not trigger.
- Most single-operation cases (multiply, exchange rate, quadratic) produce isolated warnings, consistent with localized cancellation or scaling error.

## Details

Each section shows warning samples and the first few lines of program output.

### test_combined

- Source: tests/test_combined.c
- Status: ok
- Warnings: 1
- Notes: Loss of significance when adding a small value to a large value.

Warnings (first 5):
```text
[NUMSAN] WARNING line 6: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
1.0 + 2.0 = 3.000000
1e8 + 1.0 = 100000000.0
1e10 + 1.0 = 10000000000.0
```

### test_energy_subtract

- Source: tests/test_energy_subtract.c
- Status: ok
- Warnings: 0
- Notes: No warnings emitted for this subtraction case.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
remaining = 131072.00
```

### test_exchange_rate

- Source: tests/test_exchange_rate.c
- Status: ok
- Warnings: 1
- Notes: Precision loss on a large multiply.

Warnings (first 5):
```text
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
eur = 100000008.00
```

### test_finance_fee

- Source: tests/test_finance_fee.c
- Status: ok
- Warnings: 0
- Notes: No warnings emitted for this fee addition.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
new balance = 1000000000.00
```

### test_interest_compound

- Source: tests/test_interest_compound.c
- Status: ok
- Warnings: 19750
- Notes: Repeated compounding produces persistent drift warnings.

Warnings (first 5):
```text
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
principal = 27185970.00
```

### test_multiply

- Source: tests/test_multiply.c
- Status: ok
- Warnings: 2
- Notes: Multiplying large values amplifies rounding error.

Warnings (first 5):
```text
[NUMSAN] WARNING line 3: Catastrophic cancellation detected!
[NUMSAN] WARNING line 3: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
1e8 * 1.0000001 = 100000008.0000000000
1e10 * 1.0000001 = 10000001024.0000
```

### test_parallel_resistance

- Source: tests/test_parallel_resistance.c
- Status: ok
- Warnings: 2
- Notes: Division of highly imbalanced resistances triggers warnings.

Warnings (first 5):
```text
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
parallel = 1.00000000
```

### test_path_sum

- Source: tests/test_path_sum.c
- Status: ok
- Warnings: 999998
- Notes: Long-running accumulation loop produces sustained warnings.

Warnings (first 5):
```text
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
[NUMSAN] WARNING line 4: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
total = 99.327309
```

### test_physics_scale

- Source: tests/test_physics_scale.c
- Status: ok
- Warnings: 1
- Notes: Scaling by a near-1 factor still loses precision at large magnitudes.

Warnings (first 5):
```text
[NUMSAN] WARNING line 3: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
out = 10000001024.00
```

### test_projectile

- Source: tests/test_projectile.c
- Status: ok
- Warnings: 1
- Notes: Minor cancellation in kinematic computation.

Warnings (first 5):
```text
[NUMSAN] WARNING line 5: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
height = 9509.5000
```

### test_quadratic

- Source: tests/test_quadratic.c
- Status: ok
- Warnings: 1
- Notes: Naive quadratic formula suffers cancellation on the small root.

Warnings (first 5):
```text
[NUMSAN] WARNING line 18: Catastrophic cancellation detected!
```

Program output (first 5 lines):
```text
naive: x^2 - 5x + 6 => x1=3.000000 x2=2.000000
naive: x^2 + 1e8x + 1 => x1=0.000000 x2=-100000000.000000
stable: x^2 + 1e8x + 1 => x1=-100000000.000000 x2=-0.000000
```

### test_sensor_highpass

- Source: tests/test_sensor_highpass.c
- Status: ok
- Warnings: 0
- Notes: No warnings emitted for this high-pass subtraction.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
delta = 0.125000
```

### test_signal_cancel

- Source: tests/test_signal_cancel.c
- Status: ok
- Warnings: 0
- Notes: No warnings emitted for this cancellation case.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
clean = 0.125000
```

### test_temperature_delta

- Source: tests/test_temperature_delta.c
- Status: ok
- Warnings: 0
- Notes: No warnings emitted for this delta computation.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
delta = 0.00003052
```

### test_vector_norm

- Source: tests/test_vector_norm.c
- Status: ok
- Warnings: 0
- Notes: Output overflows to inf without triggering the threshold.

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
norm = inf
```

