
import random
import time


def generate_token(timestamp):

    rng = random.Random(timestamp)

    token = bytearray()

    # Generate 16 bytes
    for i in range(16):

        value = rng.getrandbits(8)

        token.append(value)

    return bytes(token)


print("===== RESET TOKEN GENERATION =====")

# Record current Unix timestamp
T = int(time.time())

# Generate token
token = generate_token(T)

print("\nTimestamp (T):", T)

print("Token (hex):", token.hex())

print("Token Length:", len(token), "bytes")

# Save timestamp and token for verification
with open("token_data.txt", "w") as file:

    file.write(str(T) + "\n")
    file.write(token.hex() + "\n")

print("\nTimestamp and token saved to token_data.txt")
