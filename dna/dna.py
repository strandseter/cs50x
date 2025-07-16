import csv
from sys import argv


def main():
    if (len(argv) != 3):
        print("Invalid arguments")
        return

    database = load_database(argv[1])
    sequence = load_sequence(argv[2])

    matches = find_matches(database, sequence)
    profile = find_profile(database, matches)

    print(profile)


def find_profile(persons, matches):
    for row in persons:

        tmp = dict(row)
        del tmp["name"]

        if (tmp == matches):
            return row["name"]

    return "No match"


def find_matches(persons, sequence):
    matches = {}

    for row in persons:
        for col in row:
            if col == 'name' or col in matches:
                continue

            matches[col] = str(longest_match(sequence, col))

    return matches


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


def load_database(database):
    rows = []
    with open(database) as file:
        reader = csv.DictReader(file)
        for row in reader:
            rows.append(row)
    return rows


def load_sequence(sequence):
    str = ""
    with open(sequence) as file:
        str = file.read()
    return str


main()
