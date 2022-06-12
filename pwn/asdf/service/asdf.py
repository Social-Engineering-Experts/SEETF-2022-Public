import sys

blacklist = (
    "eval",
    "exec",
    "import",
    "open",
    "os",
    "read",
    "system",
    "write",
    ";",
    "+",
    "ord",
    "chr",
    "base",
    "flag",
    "replace",
    " ",
    "decode",
    "join"
)

print("Hello! Welcome to my amazing Python interpreter!")
print("You can run anything you want, but take not, there's a few blacklists!")
print("Flag is in the root directory, have fun!")

while True:
    user_input = input("Enter command: ")

    if any(keyword in user_input.lower() for keyword in blacklist):
        print("Nein!")
        sys.exit()
    else:
        try:
            exec(user_input)
        except:
            print("Your input sucks :(")
