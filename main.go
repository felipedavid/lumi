package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
)

type Lumi struct {
	errLogger *log.Logger
	hadError  bool
}

var lumi = newLumi()

func main() {
	if len(os.Args) > 2 {
		fmt.Printf("Usage: ./%s <source_file>\n", os.Args[0])
	} else if len(os.Args) == 2 {
		_ = lumi.runFile(os.Args[1])
	} else {
		lumi.prompt()
	}

	if lumi.hadError {
		os.Exit(ExDataErr)
	}
}

func newLumi() *Lumi {
	return &Lumi{
		errLogger: log.New(os.Stderr, "[ERROR] ", 0),
	}
}

func (l *Lumi) run(source string) error {
	scanner := newScanner(source)
	tokens := scanner.scanTokens()

	for _, token := range tokens {
		fmt.Printf("%+v\n", token)
	}
	return nil
}

func (l *Lumi) runFile(file string) error {
	source, err := os.ReadFile(file)
	if err != nil {
		return err
	}

	return l.run(string(source))
}

func (l *Lumi) prompt() {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Printf(">>> ")
		source, err := reader.ReadString('\n')
		if err != nil {
			break
		}
		l.run(source[:len(source)-1])
		l.hadError = false
	}
}

func (l *Lumi) error(line int, message string) {
	l.report(line, "", message)
}

func (l *Lumi) report(line int, where, message string) {
	l.errLogger.Printf("[line %d] Error %s: %s", line, where, message)
	l.hadError = false
}
