import sys
from scanner import Scanner


class Lumi:
    had_error: bool

    def __init__(self):
        self.had_error = False

    def run(self, src: str):
        scanner = Scanner(src)
        tokens = scanner.scan_tokens()

        for token in tokens:
            print(token)

    def run_file(self, file_name: str):
        with open(file_name, "r") as f:
            content = f.read()
            self.run(content)
            if self.had_error:
                sys.exit(65)

    def run_prompt(self):
        while True:
            src = input("> ")
            if src == "":
                continue

            if src == "exit":
                break

            self.run(src)
            self.had_error = False


def main():
    nargs = len(sys.argv) - 1

    lumi = Lumi()

    if nargs > 1:
        print(f"Usage: ./{sys.argv[0]} <script>")
        sys.exit(1)
    elif nargs == 1:
        lumi.run_file(sys.argv[1])
    else:
        lumi.run_prompt()


if __name__ == "__main__":
    main()
