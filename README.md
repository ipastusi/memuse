# Memuse

[![C](https://github.com/ipastusi/memuse/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/ipastusi/memuse/actions/workflows/ci.yml)

Multi-platform CLI tool for simulating memory consumption.

## Quick start guide

See GitHub Actions for details of build process and list of operating systems Memuse is tested against.

```
./memuse -h

-c      Memory allocation config
-d      Don't lock memory after allocation, might get paged to the swap area
-e      Ignore allocation and locking errors, if possible
-h      Print this help screen
-m      Use MB instead of MiB
-p      Allocate specified memory size divided into parts
-s      Don't sleep on errors, ignored unless used together with -i
-w      Wrap around and run in endless loop

Examples:
memuse -c '100:10|200:20'       Allocate 100MiB for 10 seconds, 200MiB for 20 seconds
memuse -c '100:10|200:20' -w    Allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat
memuse -c '500:10' -p 4         Allocate 500MiB split into 4 parts for 10 seconds
memuse -c '100:10' -m           Allocate 100MB for 10 seconds
```

Sample execution log:

```
./memuse -c '1024:10|2048:10|4096:10'           
allocating memory size: 1024MiB (1x 1073741824 bytes) for 10s
locking memory
sleeping
unallocating memory
allocating memory size: 2048MiB (1x 2147483648 bytes) for 10s
locking memory
sleeping
unallocating memory
allocating memory size: 4096MiB (1x 4294967296 bytes) for 10s
locking memory
sleeping
unallocating memory
exiting...
```

## Troubleshooting

Below error messages indicate memory allocation or locking failed:

- `memory allocation error`
    - Check if there is enough free memory available.
    - Consider using `-p` option.
    - Check `errno`.
- `memory locking error`
    - Check the maximum number of bytes of memory that may be locked into RAM. This is something Memuse will report, although you can also use `ulimit -l`.
    - Check if there is enough free memory available.
    - Consider using `-p` option.
    - Check `errno`.
