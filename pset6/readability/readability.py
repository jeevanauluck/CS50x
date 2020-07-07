from cs50 import get_string


def ask_for_text():
    text = get_string("Text: ")
    coleman_liau_index(text)


def letter_counter(text):
    counter = 0
    for letters in text:
        if letters.isalpha() == True:
            counter += 1
    return counter


def word_counter(text):
    words = len(text.split())  # splits words into a list
    return words


def sentence_counter(text):
    return text.count('.') + text.count('?') + text.count('!')  # counts number of respective punctuation


def coleman_liau_index(text):
    L = (letter_counter(text) / word_counter(text)) * 100
    S = (sentence_counter(text) / word_counter(text)) * 100
    index = round(0.0588 * L - 0.296 * S - 15.8)
    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


ask_for_text()