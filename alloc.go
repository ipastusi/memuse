package main

import (
	"golang.org/x/sys/unix"
	"log"
	"math"
	"strconv"
	"time"
	"unsafe"
)

/*
	#include "memuse.h"
*/
import "C"

func run(cfg Config) bool {
	unit := getUnit(cfg.isMb)
	mem := make([]unsafe.Pointer, cfg.parts)

	for {
		for _, stage := range cfg.stages {
			var memErrors bool
			size := stage.memSize
			secs := stage.seconds

			byteSize := getByteSize(stage.memSize, cfg.isMb)
			partByteSize := byteSize / cfg.parts

			log.Printf("allocating memory size: %v%v (%vx %v bytes) for %vs\n", size, unit, cfg.parts, partByteSize, secs)

			// allocate memory
			for i := 0; i < cfg.parts; i++ {
				mem[i] = C.memuse_alloc(C.size_t(partByteSize))

				if mem[i] == nil {
					log.Printf("memory allocation error")
					if !cfg.ignoreErrors {
						return false
					}
					memErrors = true
				}
			}

			if cfg.lock && !memErrors {
				log.Println("locking memory")
				// lock memory
				for i := 0; i < cfg.parts; i++ {
					if mem[i] != nil {
						_, err := C.memuse_lock(mem[i], C.size_t(partByteSize))
						if err != nil {
							log.Printf("memory locking error: %v", err)
							checkMemoryLockingLimits()
							if !cfg.ignoreErrors {
								return false
							}
							memErrors = true
						}
					}
				}
			}

			if !memErrors || cfg.sleepOnError {
				log.Println("sleeping")
				time.Sleep(time.Second * time.Duration(secs))
			}

			// unlock memory
			for i := 0; i < cfg.parts; i++ {
				if mem[i] != nil {
					C.memuse_unlock(mem[i], C.size_t(partByteSize))
				}
			}

			// unallocate memory
			log.Println("unallocating memory")
			for i := 0; i < cfg.parts; i++ {
				if mem[i] != nil {
					C.memuse_unalloc(mem[i])
				}
			}

			log.Printf("===============")
		}

		if !cfg.loop {
			break
		}
	}

	return true
}

func checkMemoryLockingLimits() {
	var rlimit unix.Rlimit
	err := unix.Getrlimit(unix.RLIMIT_MEMLOCK, &rlimit)
	if err != nil {
		log.Println("unable to determine memory locking limits")
		return
	}

	var curLimit, maxLimit string
	if rlimit.Cur == math.MaxInt {
		curLimit = "ulimited"
	} else {
		curLimit = strconv.FormatUint(rlimit.Cur, 10)
	}
	if rlimit.Max == math.MaxInt {
		maxLimit = "ulimited"
	} else {
		maxLimit = strconv.FormatUint(rlimit.Max, 10)
	}

	log.Printf("memory locking limits: %v (soft), %v (hard)\n", curLimit, maxLimit)
}

func getByteSize(size int, isMb bool) int {
	var multiplier int
	if isMb {
		multiplier = 1000 * 1000
	} else {
		multiplier = 1024 * 1024
	}
	return size * multiplier
}

func getUnit(isMb bool) string {
	if isMb {
		return "MB"
	}
	return "MiB"
}
