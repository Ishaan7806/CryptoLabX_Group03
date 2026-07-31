from collections import Counter
import os


def analyze_file(filepath):
    if not os.path.exists(filepath):
        print("Error: File not found!")
        return

    with open(filepath, "r", encoding="utf-8") as file:
        text = file.read()

    # Statistics
    num_characters = len(text)
    num_words = len(text.split())
    num_lines = len(text.splitlines())
    unique_characters = len(set(text))

    # Letter frequency (ignore case, count only alphabets)
    letters = [char.lower() for char in text if char.isalpha()]
    frequency = Counter(letters)

    print("\n===== File Analysis =====")
    print(f"Characters       : {num_characters}")
    print(f"Words            : {num_words}")
    print(f"Lines            : {num_lines}")
    print(f"Unique Characters: {unique_characters}")

    print("\nLetter Frequency:")
    for letter in sorted(frequency):
        print(f"{letter} : {frequency[letter]}")


if __name__ == "__main__":
    analyze_file("datasets/input.txt")
