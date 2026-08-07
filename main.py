from datetime import datetime
from collections import Counter
import os

def write_log(option):
    with open("logs.txt", "a") as f:
        now = datetime.now()
        f.write(f"{now.strftime('%Y-%m-%d %H:%M:%S')} - {option}\n")

def analyze_file(filepath):
    if not os.path.exists(filepath):
        print("Error: File not found!")
        return

    with open(filepath, "r", encoding="utf-8") as file:
        text = file.read()

    num_characters = len(text)
    num_words = len(text.split())
    num_lines = len(text.splitlines())
    unique_characters = len(set(text))

    letters = [char.lower() for char in text if char.isalpha()]
    frequency = Counter(letters)

    print("\n===== File Analysis =====")
    print(f"Characters        : {num_characters}")
    print(f"Words             : {num_words}")
    print(f"Lines             : {num_lines}")
    print(f"Unique Characters : {unique_characters}")

    print("\nLetter Frequency:")
    for letter in sorted(frequency):
        print(f"{letter} : {frequency[letter]}")

def menu():
    while True:
        print("\n===== CryptoLabX =====")
        print("1. Encrypt")
        print("2. Decrypt")
        print("3. Attack")
        print("4. Analyze")
        print("5. Exit")

        choice = input("Enter your choice: ")

        if choice == "1":
            write_log("Encrypt")
            print("Encrypt - Coming Soon")

        elif choice == "2":
            write_log("Decrypt")
            print("Decrypt - Coming Soon")

        elif choice == "3":
            write_log("Attack")
            print("Attack - Coming Soon")

        elif choice == "4":
            write_log("Analyze")
            analyze_file("datasets/test1.txt")

        elif choice == "5":
            write_log("Exit")
            print("Goodbye!")
            break

        else:
            print("Invalid choice")

menu()
