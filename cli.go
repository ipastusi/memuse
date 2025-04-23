package main

import (
	"flag"
	"fmt"
	"log"
	"strconv"
	"strings"
)

type Config struct {
	stages       []Stage
	lock         bool
	ignoreErrors bool
	isMb         bool
	parts        int
	sleepOnError bool
	loop         bool
}

func (c Config) String() string {
	return fmt.Sprintf("allocation config: '%v', lock: %v, ignore errors: %v, use MB: %v, parts: %v, sleep on error: %v, endless loop: %v",
		c.stages, c.lock, c.ignoreErrors, c.isMb, c.parts, c.sleepOnError, c.loop)
}

type Stage struct {
	memSize int
	seconds int
}

func parseConfig() (Config, bool) {
	c := flag.String("c", "", "Memory allocation config, e.g. '100:10|200:20' (allocate 100MiB for 10 seconds, 200MiB for 20 seconds)")
	l := flag.Bool("l", true, "Lock memory after allocation, otherwise might get paged to the swap area")
	e := flag.Bool("e", false, "Ignore allocation and locking errors, if possible")
	m := flag.Bool("m", false, "Use MB instead of MiB")
	p := flag.Int("p", 1, "Allocate specified memory size divided into parts")
	s := flag.Bool("s", true, "Sleep on errors, ignored unless used together with -e")
	w := flag.Bool("w", false, "Wrap around and run in endless loop")
	flag.Parse()

	if *p < 1 || *p > 32 {
		log.Println("-p option accepts only integer values between 1 and 32 inclusive, provided: " + strconv.Itoa(*p))
		return Config{}, false
	}

	stages, ok := parseAllocationConfig(*c)
	if !ok {
		return Config{}, false
	}

	cfg := Config{
		stages:       stages,
		lock:         *l,
		ignoreErrors: *e,
		isMb:         *m,
		parts:        *p,
		sleepOnError: *s,
		loop:         *w,
	}

	return cfg, true
}

func parseAllocationConfig(input string) ([]Stage, bool) {
	if input == "" {
		log.Println("empty allocation config")
		return []Stage{}, false
	}

	var stages []Stage

	for _, stage := range strings.Split(input, "|") {

		parts := strings.Split(stage, ":")
		if len(parts) != 2 {
			log.Println("invalid allocation config: '" + input + "'")
			return nil, false
		}

		size, err := strconv.Atoi(parts[0])
		if size < 1 || err != nil {
			log.Println("invalid allocation config: '" + input + "'")
			return nil, false
		}
		secs, err := strconv.Atoi(parts[1])
		if secs < 0 || err != nil {
			log.Println("invalid allocation config: '" + input + "'")
			return nil, false
		}

		stage := Stage{
			memSize: size,
			seconds: secs,
		}
		stages = append(stages, stage)
	}

	return stages, true
}
