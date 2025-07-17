import sys
import error
from scanner import Scanner

def run(src: str):
    scanner = Scanner(src)
    tokens = scanner.scan_tokens()

    for token in tokens:
        print(token)

def run_file(file_name: str):
    with open(file_name, "r") as f:
        content = f.read()
        run(content)
        if error.had_error:
            sys.exit(65)

def run_prompt():
    while True:
        src = input("> ")
        if src == "":
            continue

        if src == "exit":
            break

        run(src)
        error.had_error = False