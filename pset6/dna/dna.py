import csv
import re
from sys import argv, exit


def arg_checker():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit(1)


def main():
    with open(argv[2], "r") as file:  # opens sequence.txt file to count max number of STR repetitions
        sequence = file.read()
        with open(argv[1], "r") as file:  # opens csv database to access the STRs
            reader = csv.reader(file, delimiter=',')
            sequence_list = next(reader)[1:]  # creates list of sequences
        STRs = sequence_list
        pattern = '|'.join(f'({tgt})+' for tgt in STRs)

        tracker = dict.fromkeys(STRs, 0)  # kudos to Raymond Hettinger for providing the counter fcn
        # retrieved from: https://stackoverflow.com/questions/59954613/regex-counting-greatest-number-of-short-tandem-repeats
        for mo in re.finditer(pattern, sequence):
            start, end = mo.span()
            STR = next(filter(None, mo.groups()))
            reps = (end - start) // len(STR)
            tracker[STR] = max(tracker[STR], reps)
        tracker_list = list(tracker.values())  # convert max counts into a list
        print(tracker_list)
    with open(argv[1], "r") as file:  # opens csv database to access the STRs
        reader = csv.reader(file, delimiter=',')
        sequence_list = next(reader)[1:]  # creates list of sequences
        for line in reader:
            int_val = [int(i) for i in line[1:]]  # converts csv string sequence values to int via list comprehension
            if tracker_list == int_val:
                print(line[0])
                exit(0)
        print("No match")


arg_checker()
main()