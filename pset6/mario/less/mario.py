from cs50 import get_int


def main():
    i = get_inclusive_int()
    for j in range(i):  # for loop to iterate over each line
        print((" " * (i - j - 1)) + ("#" * (j + 1)), end="")
        print()


def get_inclusive_int():  # fcn to get an integer from 1 to 8 inclusive
    while True:
        n = get_int("Height: ")
        if n >= 1 and n <= 8:
            break
    return n


main()

