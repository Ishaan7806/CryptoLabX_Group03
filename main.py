from datetime import datetime

def write_log(option):
    with open("logs.txt", "a") as f:
        now = datetime.now()
        f.write(f"{now.strftime('%Y-%m-%d %H:%M:%S')} - {option}\n")

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
            print("Analyze - Coming Soon")
            # Call your Task 4 file analysis function here if you want

        elif choice == "5":
            write_log("Exit")
            print("Goodbye!")
            break

        else:
            print("Invalid choice")

menu()
