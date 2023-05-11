package main

import (
	"bufio"
	"errors"
	"fmt"
	"os"
	"strings"
)

var (
	hadError    = false
	scanningErr = errors.New("scanning error")
)

type Lumi struct {
	Lexer
}

func (l *Lumi) run(source string) error {
	l.Lexer = *newLexer(source)
	tokens, err := l.parseTokens()
	if err != nil {
		return err
	}

	for _, token := range tokens {
		fmt.Println(token.str())
	}

	return nil
}

func (l *Lumi) runPrompt() {
	reader := bufio.NewReader(os.Stdin)

	for {
		fmt.Printf(">>> ")
		source, _ := reader.ReadString('\n')
		source = strings.Replace(source, "\n", "", -1)

		l.run(source)
		hadError = false
	}
}

func (l *Lumi) runFile(file string) error {
	source, err := os.ReadFile(file)
	if err != nil {
		return err
	}

	return l.run(string(source))
}

func main() {
	var lumi Lumi

	if len(os.Args) > 2 {
		fmt.Println("Usage: ./lumi <script>")
	} else if len(os.Args) == 2 {
		err := lumi.runFile(os.Args[1])
		if err != nil {
			switch err {
			case ErrUnexpectedCharacter:
				fmt.Printf("Lexer: unexpected character '%c' at line %d\n", lumi.source[lumi.current-1], lumi.line)
			case ErrUnterminatedString:
				fmt.Printf("Lexer: unterminated string at line %d\n", lumi.line)
			}
		}
	} else {
		lumi.runPrompt()
	}
}
