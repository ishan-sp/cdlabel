# Evaluation Report

Date: 2026-06-01

This report summarizes running numsan across all programs in tests/.

## Summary

| Test | Status | Warnings | Notes |
| --- | --- | --- | --- |
| test_combined | ok | 1 | 1.0 + 2.0 = 3.000000 |
| test_energy_subtract | ok | 0 | remaining = 131072.00 |
| test_exchange_rate | ok | 1 | eur = 100000008.00 |
| test_finance_fee | ok | 0 | new balance = 1000000000.00 |
| test_interest_compound | ok | 19750 | principal = 27185970.00 |
| test_multiply | ok | 2 | 1e8 * 1.0000001 = 100000008.0000000000 |
| test_parallel_resistance | ok | 2 | parallel = 1.00000000 |
| test_path_sum | ok | 999998 | total = 99.327309 |
| test_physics_scale | ok | 1 | out = 10000001024.00 |
| test_projectile | ok | 1 | height = 9509.5000 |
| test_quadratic | ok | 1 | naive: x^2 - 5x + 6 => x1=3.000000 x2=2.000000 |
| test_sensor_highpass | ok | 0 | delta = 0.125000 |
| test_signal_cancel | ok | 0 | clean = 0.125000 |
| test_temperature_delta | ok | 0 | delta = 0.00003052 |
| test_vector_norm | ok | 0 | norm = inf |

## Details

Each section shows warning samples and the first few lines of program output.

### test_combined

- Source: tests/test_combined.c
- Status: ok
- Warnings: 1

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

Warnings (first 5):
(none)

Program output (first 5 lines):
```text
norm = inf
```

