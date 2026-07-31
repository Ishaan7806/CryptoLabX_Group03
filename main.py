def encrypt():
    print("Coming Soon")

def decrypt():
    print("Coming Soon")

def attack():
    print("Coming Soon")

def analyze():
    print("Coming Soon")

def menu():
    while True:
        print("\n===== CryptoLabX =====")
        print("1. Encrypt")
        print("2. Decrypt")
        print("3. Attack")
        print("4. Analyze")
        print("5. Exit")

        choice = input("Enter choice: ")

        if choice=="1":
            encrypt()

        elif choice=="2":
            decrypt()

        elif choice=="3":
            attack()

        elif choice=="4":
            analyze()

        elif choice=="5":
            break

        else:
            print("Invalid Choice")

menu()
