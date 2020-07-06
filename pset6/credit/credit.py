from cs50 import get_string
from cs50 import get_int


def luhn():
    card_number = get_int("Number: ")
    reversed_card_number = str(card_number)[::-1]
    reversed_card_number = [int(i) for i in reversed_card_number]  # convert to int list using list comprehension

    # step 1: Multiply every other digit by 2, starting with the number’s second-to-last digit, and then add those products’ digits together.
    second_last_digit = reversed_card_number[1::2]
    doubled_list = [2*i for i in second_last_digit]
    string_doubled_list = ''.join(str(j) for j in doubled_list)  # converts list to string
    sum_of_products_digit_list = sum([int(digits) for digits in string_doubled_list])

    # step 2: Add the sum to the sum of the digits that weren’t multiplied by 2
    both_sums = sum_of_products_digit_list + sum(reversed_card_number[0::2])

    # step 3: If the total’s last digit is 0 (or, put more formally, if the total modulo 10 is congruent to 0), the number is valid!
    if (both_sums % 10 == 0):
        string_card_number = str(card_number)
        num_of_card_number_digits = len(str(card_number))
        # AMEX are 15 numbas long and start with 34 or 37
        if num_of_card_number_digits == 15 and ((string_card_number[0] == '3') and (string_card_number[1] == '4' or '7')):
            print("AMEX")
        elif num_of_card_number_digits == 16 and ((string_card_number[0] == '5') and (string_card_number[1] == '1' or '2' or '3' or '4' or '5')):
            print("MASTERCARD")
        elif (num_of_card_number_digits == 13 or 17) and (string_card_number[0] == '4'):
            print("VISA")
    else:
        print("INVALID")


luhn()