package main

import (
	"log"
	"os"
	"os/signal"
	"syscall"
)

func main() {
	log.SetOutput(os.Stdout)
	log.Println("starting memuse")
	registerSignalHandler()

	cfg, ok := parseConfig()
	if !ok {
		log.Println("error, exiting")
		os.Exit(1)
	}

	log.Println(cfg.String())
	ok = run(cfg)
	if !ok {
		log.Println("error, exiting")
		os.Exit(1)
	}
	log.Println("exiting")
}

func registerSignalHandler() {
	sigs := make(chan os.Signal, 1)
	signal.Notify(sigs, syscall.SIGINT, syscall.SIGTERM)
	go func() {
		sig := <-sigs
		log.Printf("received signal: %v, exiting\n", sig)
		os.Exit(0)
	}()
}
