# memuse

CLI tool simulating memory consumption.

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

See GitHub Actions for the details of build process and list of operating systems memuse is tested against.

## Errors

Common error messages:
- `memory allocation error` - check if there is enough free memory available.
- `memory locking error` - use `ulimit -l` to check the maximum number of bytes of memory that may be locked into RAM.
