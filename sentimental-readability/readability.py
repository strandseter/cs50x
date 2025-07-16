def main():
    _input = input("Text: ")

    letter_count = count_letters(_input)
    word_count = count_words(_input)
    sentence_count = count_sentences(_input)

    index = calculate_index(letter_count, word_count, sentence_count)

    result = get_result(index)

    print(result)


def get_result(index):
    if (index >= 16):
        return "Grade 16+"

    if (index < 1):
        return "Before Grade 1"

    return f"Grade {index}"


def calculate_index(letters, words, sentences):
    l = letters / words * 100
    s = sentences / words * 100

    ret = (0.0588 * l) - (0.296 * s) - 15.8

    return round(ret)


def count_letters(text):
    return sum(c.isalpha() for c in text)


def count_words(text):
    return len(text.split(" "))


def count_sentences(text):
    return sum(text.count(ending) for ending in ['.', '!', '?'])


main()
