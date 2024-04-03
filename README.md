# Memuse

[![C](https://github.com/ipastusi/memuse/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/ipastusi/memuse/actions/workflows/ci.yml)

Multi-platform CLI tool for simulating memory consumption.

## Quick start guide

```
./memuse -h

-a      Ignore allocation errors
-c      Memory allocation config
-d      Don't lock memory after allocation, might get paged to the swap area
-h      Print this help screen
-l      Ignore locking errors
-p      Allocate specified memory size divided into parts
-m      Use MB instead of MiB
-s      Don't sleep on error, ignored unless used together with -a or -l
-w      Wrap around and run in endless loop

Examples:
memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds
memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat the sequence until interrupted
memuse -c '500:10' -p 4         Allocate 500MiB split into 4 parts for 10 seconds
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
