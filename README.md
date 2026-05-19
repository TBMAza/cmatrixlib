# matrixlibc

A simple matrix operations library written in C, with both single-threaded and multithreaded implementations.

## What it does

Provides basic matrix operations:

- **Initialization / print / free** — `matinit`, `matprint`, `matfree`
- **Transpose** — `mattranspose`
- **Element-wise sum** — `matsum` / `matsum_m`
- **Scalar multiplication** — `matscalarmult` / `matscalarmult_m`
- **Matrix multiplication** — `matmult` / `matmult_m` / `matmult_m_tr`

Functions ending in `_m` are multithreaded (pthreads). `matmult_m_tr` is a cache-friendlier variant that transposes the second operand before multiplying, which tends to be faster for large matrices.

The multithreaded functions fall back to their single-threaded counterparts automatically when the matrix is small enough that threading wouldn't be worth it (configurable via `MINELEMSXTHREAD` in the header).

## Configuration

A couple of constants in `matrixlibc.h` you might want to tweak:

| Constant | Default | Description |
|---|---|---|
| `MAXTHREADS` | 8 | Maximum number of worker threads |
| `MINELEMSXTHREAD` | 262144 | Minimum elements per thread before multithreading kicks in |

## Status

Early stages — more operations and improvements to come.
