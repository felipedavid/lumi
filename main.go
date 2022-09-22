package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

var hadError = false

func main() {
	if len(os.Args) > 2 {
		fmt.Println("Usage: ./lumi [script]")
		os.Exit(64)
	} else if len(os.Args) == 2 {
		err := runFile(os.Args[1])
		if err != nil {
			os.Exit(65)
		}
	} else {
		runPrompt()
	}
}

func runFile(filename string) error {
	source, err := os.ReadFile(filename)
	if err != nil {
		return err
	}

	return run(string(source))
}

func runPrompt() {
	reader := bufio.NewReader(os.Stdin)
	for {
		fmt.Printf(">>> ")
		line, _ := reader.ReadString('\n')
		if line == "" {
			break
		}
		line = strings.Replace(line, "\n", "", -1)
		run(line)
		hadError = false
	}
}

func run(source string) error {
	s := newScanner(source)
	tokens := s.scanTokens()

	for _, t := range tokens {
		fmt.Println(t.String())
	}

	return nil
}

func reportError(line int, message string) {
	hadError = true
	fmt.Printf("[line %d] Error %s: %s", line, "", message)
}
