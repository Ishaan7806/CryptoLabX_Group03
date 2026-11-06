
import os

SIZE = 1024 * 1024

OUTPUT_FILE = "data/linux_urandom.bin"

os.makedirs("data", exist_ok=True)

with open("/dev/urandom", "rb") as source:
    random_data = source.read(SIZE)

with open(OUTPUT_FILE, "wb") as output:
    output.write(random_data)

print("Linux random data generated successfully.")
print("File:", OUTPUT_FILE)
print("Size:", len(random_data), "bytes")
