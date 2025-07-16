from cs50 import get_int


def main():
    credit_number = get_int("Number: ")

    is_valid = isNumberValid(credit_number)

    if not is_valid:
        print("INVALID")
        return

    provider = find_provider(credit_number)

    print(provider)


def isNumberValid(number):
    number_str = str(number)
    total_sum = 0

    for i, digit_str in enumerate(number_str[::-1]):
        digit = int(digit_str)

        if i % 2 == 0:
            total_sum += digit
        else:
            doubled = digit * 2
            total_sum += (doubled // 10) + (doubled % 10)

    return total_sum % 10 == 0


def find_provider(number):
    num_str = str(number)

    length = len(num_str)
    ftd = int(num_str[0] + num_str[1])
    fd = int(num_str[0])

    is_amex = length == 15 and (ftd == 34 or ftd == 37)
    is_mastercard = length == 16 and ftd >= 51 and ftd <= 55
    is_visa = (length == 13 or length == 16) and fd == 4

    print(is_amex)

    if is_amex:
        return 'AMEX'

    if is_mastercard:
        return 'MASTERCARD'

    if is_visa:
        return 'VISA'

    return 'INVALID'


main()
