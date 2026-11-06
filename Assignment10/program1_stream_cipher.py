import random
import secrets
import string


# Generate keystream and XOR with data
def encrypt_decrypt(data, seed):

    rng = random.Random(seed)
    result = bytearray()

    for byte in data:
        key = rng.getrandbits(8)
        result.append(byte ^ key)

    return bytes(result)


# Part 1: Test encryption and decryption
print("===== MT19937 STREAM CIPHER =====")

message = "Hello, this is a test message!"

seed = 12345

plaintext = message.encode()

ciphertext = encrypt_decrypt(plaintext, seed)

decrypted = encrypt_decrypt(ciphertext, seed)

print("\nOriginal Message:", message)
print("Seed:", seed)
print("Ciphertext (hex):", ciphertext.hex())
print("Decrypted Message:", decrypted.decode())

print("\nEncryption/Decryption Successful:",
      decrypted == plaintext)


# Part 2: Generate challenge plaintext
print("\n===== CHALLENGE GENERATION =====")

prefix_length = secrets.randbelow(16) + 5

characters = string.ascii_letters + string.digits + string.punctuation

prefix = ''.join(
    secrets.choice(characters)
    for _ in range(prefix_length)
)

challenge_plaintext = prefix + "A" * 14

challenge_seed = secrets.randbelow(65536)

challenge_ciphertext = encrypt_decrypt(
    challenge_plaintext.encode(),
    challenge_seed
)

print("\nChallenge Plaintext:", challenge_plaintext)
print("Challenge Seed (creator only):", challenge_seed)
print("Challenge Ciphertext (hex):",
      challenge_ciphertext.hex())

print("\nSave the ciphertext for Program 2.")