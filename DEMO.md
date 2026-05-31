# NumSan Studio Demo

## Overview
NumSan Studio highlights numerically unstable operations in C code by pairing line-level warnings with output context. These screenshots show two classic cases where subtraction between close values loses precision.

## Demo 1: Quadratic Solver Cancellation
This example compares a naive quadratic formula to a more stable formulation. The warning panel flags a cancellation at the subtraction step, and the output shows the drift between the float result and a double-precision shadow.

![Quadratic cancellation warning](docs/images/demo-quadratic.png)

## Demo 2: Projectile Height Cancellation
Here, the height calculation mixes a large positive term with a similar magnitude negative term. NumSan Studio detects the loss of significant digits and surfaces the warning alongside the computed height.

![Projectile cancellation warning](docs/images/demo-projectile.png)

## Takeaway
NumSan Studio makes precision hazards visible where they happen, so you can quickly replace unstable formulas with numerically robust alternatives.
