# vdl

## Files

| File | Description |
|:---:|:---:|
| `vdldef.h` | Definition of vector. |
| `vdlutil.h` | Utility macros. |
| `vdlerr.h` | Error and exception handling. |
| `vdlbt.h` | Backtrace. |
| `vdlassert.h` | Safety checks. |
| `vdlwrapper.h` | Low level helper macros and functions to interact with vectors. |
| `vdlgc.h` | Garbage collector. |
| `vdlmem.h` | Memory management interface. |

## Naming convention

- `vdl_*`: Normal functions/macros.
- `vdl_*_BT`: Functions with backtrace enabled. Only for internal use.
- `vdlint_*`: Internal functions/macros.
- `vdl_e_*`/`vdlint_e_*`: Functions/macros that could throw an exception.
- `vdl_ne_*`/`vdlint_ne_*`: Functions/macros that could abort the program (no exception).
