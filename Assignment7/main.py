from oracle import create_challenge, padding_oracle
from attack import padding_oracle_attack


def main():
    iv, ciphertext = create_challenge()

    print("=== Padding Oracle Attack Demo ===")
    print("Ciphertext length:", len(ciphertext), "bytes")
    print("IV:", iv.hex())
    print("Ciphertext:", ciphertext.hex())
    print("\nRecovering plaintext...")

    plaintext, queries = padding_oracle_attack(
        iv,
        ciphertext,
        padding_oracle
    )

    print("\nRecovered plaintext:")
    try:
        print(plaintext.decode("utf-8"))
    except UnicodeDecodeError:
        print(plaintext)

    print("\nTotal oracle queries:", queries)


if __name__ == "__main__":
    main()
