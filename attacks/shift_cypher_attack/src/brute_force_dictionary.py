# brute_force_dictionary.py

from shift_cipher import decrypt


def load_dictionary(filename):
    words = set()

    try:
        with open(filename, "r", encoding="utf-8") as file:

            for line in file:
                word = line.strip().upper()

                if word:
                    words.add(word)

    except FileNotFoundError:
        print("Error: Dictionary file not found!")
        return set()

    return words


def dictionary_score(text, dictionary):

    words = text.upper().split()

    score = 0

    for word in words:

        clean_word = ""

        for char in word:

            if char.isalpha():
                clean_word += char

        if clean_word in dictionary:
            score += 1

    return score


def dictionary_attack(ciphertext, dictionary):

    best_key = 0
    best_plaintext = ""
    best_score = -1

    for key in range(26):

        plaintext = decrypt(ciphertext, key)

        score = dictionary_score(plaintext, dictionary)

        if score > best_score:

            best_score = score
            best_key = key
            best_plaintext = plaintext

    return best_key, best_plaintext, best_score


if __name__ == "__main__":

    ciphertext = "AOL XBPJR IYVDU MVE QBTWZ VCLY AOL SHGF KVN"

    dictionary = load_dictionary("../dictionary/english_text.txt")

    key, plaintext, score = dictionary_attack(
        ciphertext,
        dictionary
    )

    print("===== BRUTE FORCE + DICTIONARY ATTACK =====")
    print("Predicted Key      :", key)
    print("Predicted Plaintext:", plaintext)
    print("Dictionary Score   :", score)
