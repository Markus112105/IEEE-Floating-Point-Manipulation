# Extended Floating Point Project - Floatx

## Overview

This project explores IEEE floating-point representation and custom floating-point formats. Unlike standard IEEE float (32-bit) or double (64-bit) types, our generalized format, "floatx," allows flexible bit allocation for precision and range. Specifically, `floatx` numbers use configurable bit-widths for the exponent and fraction fields, enabling formats like `floatx(12,5)`—12 total bits with 5 for the exponent.

The `doubleToFloatx` function converts a double-precision value to this custom format, maintaining key IEEE floating-point conventions:

- **Sign bit**: Determines positivity or negativity.
- **Exponent field**: Biased representation with configurable size.
- **Fraction field**: Varying precision based on total and exponent bits.

## Key Features and Functionality

- **Flexible bit-widths**: Support for various total bit and exponent sizes, from `floatx(3,1)` to `floatx(64,62)`.
- **IEEE standards**: Follows conventions for sign, exponent biasing, normalization, special cases (e.g., NaN, infinity).
- **Bit manipulation**: Utilizes custom utilities for extracting and managing bits.

This format is well-suited for applications requiring high precision but limited range. With `floatx`, users can tailor floating-point representation to the needs of specialized tasks.

