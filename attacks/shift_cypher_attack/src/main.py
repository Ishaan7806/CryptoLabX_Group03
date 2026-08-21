from shift_cipher import encrypt, decrypt
from brute_force_dictionary import load_dictionary, dictionary_attack
from chi_square_attack import chi_square_attack


def main():

    print("===== SHIFT CIPHER CRYPTANALYSIS =====")

    plaintext = input("Enter plaintext: ")
    actual_key = int(input("Enter key (0-25): "))

    ciphertext = encrypt(plaintext, actual_key)

    print("\nOriginal Plaintext:")
    print(plaintext)

    print("\nActual Key:")
    print(actual_key)

    print("\nCiphertext:")
    print(ciphertext)

    dictionary = load_dictionary("../dictionary/english_words.txt")

    dictionary_key, dictionary_plaintext, dictionary_score_value = dictionary_attack(
        ciphertext,
        dictionary
    )

    chi_key, chi_plaintext, chi_score = chi_square_attack(ciphertext)

    print("\n========== DICTIONARY ATTACK ==========")
    print("Predicted Key:", dictionary_key)
    print("Decrypted Text:", dictionary_plaintext)
    print("Dictionary Score:", dictionary_score_value)

    print("\n========== CHI-SQUARE ATTACK ==========")
    print("Predicted Key:", chi_key)
    print("Decrypted Text:", chi_plaintext)
    print("Chi-Square Score:", chi_score)

    print("\n========== COMPARISON ==========")

    print("Actual Key:", actual_key)
    print("Dictionary Key:", dictionary_key)
    print("Chi-Square Key:", chi_key)

    if dictionary_key == actual_key:
        print("Dictionary Correct: Yes")
    else:
        print("Dictionary Correct: No")

    if chi_key == actual_key:
        print("Chi-Square Correct: Yes")
    else:
        print("Chi-Square Correct: No")


if __name__ == "__main__":
    main()
