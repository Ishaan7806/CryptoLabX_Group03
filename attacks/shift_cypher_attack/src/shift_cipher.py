# shift_cipher.py

import string

ALPHABET = string.ascii_uppercase


def encrypt(text, key):
    result = ""

    for char in text:

        if char.isalpha():

            char = char.upper()

            index = ALPHABET.index(char)

            new_index = (index + key) % 26

            result += ALPHABET[new_index]

        else:
            result += char

    return result


def decrypt(text, key):
    result = ""

    for char in text:

        if char.isalpha():

            char = char.upper()

            index = ALPHABET.index(char)

            new_index = (index - key) % 26

            result += ALPHABET[new_index]

        else:
            result += char

    return result


if __name__ == "__main__":

    plaintext = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG"
    key = 7

    ciphertext = encrypt(plaintext, key)

    print("===== SHIFT CIPHER =====")
    print("Plaintext :", plaintext)
    print("Key       :", key)
    print("Ciphertext:", ciphertext)

    decrypted = decrypt(ciphertext, key)

    print("Decrypted :", decrypted)
