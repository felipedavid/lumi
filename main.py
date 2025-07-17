import sys
from lumi import run_file, run_prompt

def main():
    nargs = len(sys.argv) - 1

    if nargs > 1:
        print(f"Usage: ./{sys.argv[0]} <script>")
        sys.exit(1)
    elif nargs == 1:
        run_file(sys.argv[1])
    else:
        run_prompt()


if __name__ == "__main__":
    main()
