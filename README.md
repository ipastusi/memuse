# Memuse

[![Go](https://github.com/ipastusi/memuse/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/ipastusi/memuse/actions/workflows/ci.yml)

Go/C (cgo) application for simulating memory consumption, initially designed for investigating Kubernetes resource limits.

## Quick start guide

Help:

```
./memuse -h

Usage of ./memuse:
  -c string
    	Memory allocation config, e.g. '100:10|200:20' (allocate 100MiB for 10 seconds, 200MiB for 20 seconds)
  -e	Ignore allocation and locking errors, if possible
  -l	Lock memory after allocation, otherwise might get paged to the swap area (default true)
  -m	Use MB instead of MiB
  -p int
    	Allocate specified memory size divided into parts (default 1)
  -s	Sleep on errors, ignored unless used together with -e (default true)
  -w	Wrap around and run in endless loop
```

Examples:

```
./memuse -c '100:10|200:20'       # allocate 100MiB for 10 seconds, 200MiB for 20 seconds
./memuse -c '100:10|200:20' -w    # allocate 100MiB for 10 seconds, 200MiB for 20 seconds, repeat
./memuse -c '500:10' -p 4         # allocate 500MiB split into 4 parts for 10 seconds
./memuse -c '100:10' -m           # allocate 100MB for 10 seconds
```

Sample execution log (timestamps removed):

```
./memuse -c '1024:10|2048:10|4096:10'
starting memuse
allocation config: '[{1024 10} {2048 10} {4096 10}]', lock: true, ignore errors: false, use MB: false, parts: 1, sleep on error: true, endless loop: false
allocating memory size: 1024MiB (1x 1073741824 bytes) for 10s
locking memory
sleeping
unallocating memory
===============
allocating memory size: 2048MiB (1x 2147483648 bytes) for 10s
locking memory
sleeping
unallocating memory
===============
allocating memory size: 4096MiB (1x 4294967296 bytes) for 10s
locking memory
sleeping
unallocating memory
===============
exiting
```

## Deep dive

Memuse is currently implemented in Go/C (cgo). Total memory usage will be a few MB higher that specified in the allocation config,
to accommodate Go runtime.

If execution fails with `memory allocation error`, make sure there is enough free memory available or consider using `-p` option.

If execution fails with `memory locking error`, check the maximum number of bytes of memory that may be locked into RAM. This is something
Memuse will report, although you can also use `ulimit -l`.

## Kubernetes

You may want to model your `Dockerfile` after one of the examples provided. You may also model your YAML manifest after one of the
examples provided, e.g.:

```yaml
apiVersion: v1
kind: Pod
...
spec:
  containers:
    - name: memuse-distroless
      ...
      args: [ "-c", "40:20|80:20", "-w" ]
  ...
```

In this case there is no need to specify `pod.spec.containers.command`, as the container image `ENTRYPOINT` will be used.

If you want to run Memuse and lock memory size greater than the memory locking limit on your cluster worker nodes, you might prefer to use
the following template instead:

```yaml
apiVersion: v1
kind: Pod
...
spec:
  containers:
    - name: memuse-ubuntu
      ...
      securityContext:
        runAsUser: 0
        capabilities:
          drop: [ "ALL" ]
          add: [ "SYS_RESOURCE" ]
      command: [ "bash", "-c", "ulimit -l unlimited; ./memuse -c '200:20|400:20|800:20' -w" ]
```

See the sample YAML files for more details.
