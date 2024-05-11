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
-s      Don't sleep on errors, ignored unless used together with -e
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
starting memuse           
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

## Deep dive

If execution fails with `memory allocation error`, make sure there is enough free memory available, check reported `errno` or consider using `-p` option.

If execution fails with `memory locking error`, check the maximum number of bytes of memory that may be locked into RAM. This is something Memuse will report,
although you can also use `ulimit -l`.

## Kubernetes

You can use the `Dockerfile` provided together with the below Kubernetes pod template to run Memuse on your cluster:

```yaml
apiVersion: v1
kind: Pod
...
spec:
  containers:
    - name: memuse
      ...
      args: [ "-c", "2:10|4:10", "-w" ]
  ...
```

In this case there is no need to specify `pod.spec.containers.command`, as the container image `ENTRYPOINT` will be used.

If you want to run Memuse and lock memory size greater than the memory locking limit on your cluster worker nodes, you might prefer to use the following
template instead:

```yaml
apiVersion: v1
kind: Pod
...
spec:
  containers:
    - name: memuse
      ...
      securityContext:
        runAsUser: 0
        capabilities:
          drop: [ "ALL" ]
          add: [ "SYS_RESOURCE" ]
      command: [ "bash", "-c", "ulimit -l unlimited; ./memuse -c '400:10|800:10' -w" ]
```
