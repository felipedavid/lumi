package main

import (
    "fmt"
    "os"
)

var hadError = false

func main() {
    if len(os.Args) > 2 {
        fmt.Println("Usage: lumi <script>")
        os.Exit(64)
    } else if len(os.Args) == 2 {
        runFile(os.Args[1])
    } else {
        runPrompt()
    }
}

func runPrompt() {
    var line string

    for {
        fmt.Print("> ")
        if n, _ := fmt.Scanln(&line); n == 0 {
            break
        }
        run(line)
        hadError = false
    }
}

func runFile(sourcePath string) {
    src, err := os.ReadFile(sourcePath)
    if err != nil {
        fmt.Fprintf(os.Stderr, "[!] Can't read file '%v' (%v)", sourcePath, err)
        os.Exit(1)
    }
    run(string(src))

    if hadError {
        os.Exit(65)
    }
}

func run(src string) {
    var scanner Scanner
    scanner.init(src)

    tokens := scanner.scanTokens()
    for t := range tokens {
        fmt.Println(t)
    }
}

func erro(line int, message int) {
    report(line, "", message)
}

func reportError(line int, where string, message string) {
    fmt.Fprintf("[line %v] Error%v: %v\n", line, where, message)
    hadError = true
}
