
import random


def generate_token(timestamp):

    rng = random.Random(timestamp)

    token = bytearray()

    for i in range(16):

        value = rng.getrandbits(8)

        token.append(value)

    return bytes(token)


print("===== RESET TOKEN PREDICTION =====")

# Read observed token
observed_hex = input("\nEnter observed token (hex): ").strip()

observed_token = bytes.fromhex(observed_hex)

if len(observed_token) != 16:
    raise ValueError("Token must contain exactly 16 bytes.")

# Read approximate timestamp
approx_time = int(input("Enter approximate timestamp: "))

# Search interval
start = approx_time - 300
end = approx_time + 300

tested = 0
recovered_timestamp = None

print("\nSearching timestamps...")
print("Search interval:", start, "to", end)

# Try every timestamp
for timestamp in range(start, end + 1):

    tested += 1

    candidate_token = generate_token(timestamp)

    if candidate_token == observed_token:

        recovered_timestamp = timestamp

        break


print("\n===== ATTACK RESULTS =====")

print("Candidates Tested:", tested)

if recovered_timestamp is not None:

    print("\nTimestamp Recovered:",
          recovered_timestamp)

    # Regenerate token using recovered timestamp
    verification_token = generate_token(
        recovered_timestamp
    )

    print("Recovered Token:",
          verification_token.hex())

    print("Original Token:",
          observed_token.hex())

    print("\nToken Verification:",
          verification_token == observed_token)

else:

    print("\nNo matching timestamp found.")

    print("Try a wider search interval.")
