package main

import (
	"errors"
	"fmt"
	"io"
	"os"
)

func main() {
	if len(os.Args) > 2 {
		fmt.Printf("Usage: ./%s <source_file>\n", os.Args[0])
	} else if len(os.Args) == 2 {
		_ = runFile(os.Args[1])
	} else {
		prompt()
	}
}

func run(source []byte) error {
	scanner := newScanner(source)
	tokens, err := scanner.Scan()
	if err != nil {
		return err
	}

	for _, token := range tokens {
		fmt.Printf("%v\n", token)
	}
	return nil
}

func runFile(file string) error {
	source, err := os.ReadFile(file)
	if err != nil {
		return err
	}

	return run(source)
}

func prompt() {
	var source []byte
	for {
		fmt.Printf(">>> ")
		_, err := fmt.Scanf("%s\n", &source)
		if errors.Is(err, io.EOF) {
			break
		}
		run(source)
	}
}
