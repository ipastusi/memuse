# Memuse

[![C](https://github.com/ipastusi/memuse/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/ipastusi/memuse/actions/workflows/ci.yml)

Multi-platform CLI tool for simulating memory consumption.

## Quick start guide

```
./memuse -h

-c      Memory allocation config
-h      Print this help screen
-m      Use MB instead of MiB
-w      Wrap around and run in endless loop

Examples:
memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds
memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat the sequence until interrupted
memuse -c '100:10' -m           Allocate 100MB for 10 seconds
```

See GitHub Actions for details of build process and list of operating systems Memuse is tested against.

## Troubleshooting

Common error messages:

- `memory allocation error`
  - Check if there is enough free memory available.
  - Check `errno`.
- `memory locking error`
  - Use `ulimit -l` to check the maximum number of bytes of memory that may be locked into RAM.
  - Check if there is enough free memory available.
  - Check `errno`.
