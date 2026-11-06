import os
import math
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from pathlib import Path
from scipy.stats import chisquare

# --------------------------------------------------
# 1. SETUP
# --------------------------------------------------

ROOT = Path.cwd()

if ROOT.name == "notebooks":
    ROOT = ROOT.parent

DATA_DIR = ROOT / "data"
RESULT_DIR = ROOT / "results"

RESULT_DIR.mkdir(exist_ok=True)

FILES = {
    "Linux /dev/urandom": DATA_DIR / "linux_urandom.bin",
    "OpenSSL rand": DATA_DIR / "openssl_rand.bin",
    "Windows CNG": DATA_DIR / "windows_cng.bin"
}

# --------------------------------------------------
# 2. LOAD RANDOM DATA
# --------------------------------------------------

datasets = {}

for name, path in FILES.items():

    if not path.exists():
        print("Missing file:", path)
        continue

    data = np.fromfile(path, dtype=np.uint8)

    if len(data) != 1024 * 1024:
        raise ValueError(f"{name}: Expected exactly 1 MB")

    datasets[name] = data

    print(name)
    print("Bytes:", len(data))
    print("Size:", len(data) / 1024, "KB")
    print()

if len(datasets) != 3:
    raise ValueError("All three random data files are required.")

# Store final results
results = []


# --------------------------------------------------
# 3. BYTE FREQUENCY DISTRIBUTION
# --------------------------------------------------

plt.figure(figsize=(12, 6))

for name, data in datasets.items():

    frequencies = np.bincount(data, minlength=256)

    plt.plot(
        range(256),
        frequencies,
        label=name,
        alpha=0.8
    )

plt.axhline(
    len(next(iter(datasets.values()))) / 256,
    linestyle="--",
    label="Expected frequency"
)

plt.title("Byte Frequency Distribution")
plt.xlabel("Byte Value (0-255)")
plt.ylabel("Frequency")
plt.legend()
plt.grid(alpha=0.3)

plt.savefig(RESULT_DIR / "byte_frequency.png")
plt.show()


# --------------------------------------------------
# 4. BIT DISTRIBUTION
# --------------------------------------------------

plt.figure(figsize=(10, 6))

for name, data in datasets.items():

    bits = np.unpackbits(data)

    zero_count = np.count_nonzero(bits == 0)
    one_count = np.count_nonzero(bits == 1)

    plt.bar(
        [name + " 0", name + " 1"],
        [zero_count, one_count],
        label=name
    )

    print(name)
    print("Zero bits:", zero_count)
    print("One bits:", one_count)
    print()

plt.title("Bit Distribution")
plt.ylabel("Number of Bits")
plt.xticks(rotation=20)
plt.tight_layout()

plt.savefig(RESULT_DIR / "bit_distribution.png")
plt.show()


# --------------------------------------------------
# 5. REPEATED PATTERN DETECTION
# --------------------------------------------------

BLOCK_SIZE = 16

for name, data in datasets.items():

    blocks = data[:len(data) // BLOCK_SIZE * BLOCK_SIZE]

    blocks = blocks.reshape(-1, BLOCK_SIZE)

    unique_blocks = np.unique(blocks, axis=0)

    total_blocks = len(blocks)

    repeated_count = total_blocks - len(unique_blocks)

    print(name)
    print("Total 16-byte blocks:", total_blocks)
    print("Unique blocks:", len(unique_blocks))
    print("Repeated blocks:", repeated_count)
    print()


# --------------------------------------------------
# 6. SERIAL CORRELATION
# --------------------------------------------------

for name, data in datasets.items():

    correlation = np.corrcoef(
        data[:-1].astype(np.float64),
        data[1:].astype(np.float64)
    )[0, 1]

    print(name)
    print("Lag-1 serial correlation:", correlation)
    print()

    results.append({
        "Generator": name,
        "Serial Correlation": correlation
    })


# --------------------------------------------------
# 7. CHI-SQUARE BYTE FREQUENCY TEST
# --------------------------------------------------

for name, data in datasets.items():

    observed = np.bincount(data, minlength=256)

    expected = np.full(256, len(data) / 256)

    statistic, p_value = chisquare(
        observed,
        expected
    )

    print(name)
    print("Chi-square statistic:", statistic)
    print("P-value:", p_value)
    print("Interpretation:",
          "No significant deviation detected"
          if p_value >= 0.01
          else "Significant deviation detected")
    print()

    results.append({
        "Generator": name,
        "Chi-square statistic": statistic,
        "Chi-square p-value": p_value
    })


# --------------------------------------------------
# 8. MONOBIT FREQUENCY TEST
# --------------------------------------------------

for name, data in datasets.items():

    bits = np.unpackbits(data).astype(np.int8)

    n = len(bits)

    # Convert 0 to -1 and 1 to +1
    signed_bits = 2 * bits - 1

    S = np.sum(signed_bits)

    s_obs = abs(S) / math.sqrt(n)

    p_value = math.erfc(s_obs / math.sqrt(2))

    print(name)
    print("Monobit test p-value:", p_value)
    print()

    results.append({
        "Generator": name,
        "Monobit p-value": p_value
    })


# --------------------------------------------------
# 9. RUNS TEST
# --------------------------------------------------

for name, data in datasets.items():

    bits = np.unpackbits(data)

    n = len(bits)

    pi = np.mean(bits)

    # Number of transitions + 1
    runs = 1 + np.count_nonzero(bits[1:] != bits[:-1])

    if abs(pi - 0.5) >= 2 / math.sqrt(n):

        p_value = float("nan")

        print(name)
        print("Runs test not applicable:")
        print("Bit proportion is too far from 0.5")

    else:

        expected_runs = 2 * n * pi * (1 - pi)

        denominator = (
            2 * math.sqrt(2 * n) * pi * (1 - pi)
        )

        z = abs(runs - expected_runs) / denominator

        p_value = math.erfc(z)

        print(name)
        print("Observed runs:", runs)
        print("Expected runs:", expected_runs)
        print("Runs test p-value:", p_value)

    print()

    results.append({
        "Generator": name,
        "Runs test p-value": p_value
    })


# --------------------------------------------------
# 10. SAVE RESULTS
# --------------------------------------------------

df = pd.DataFrame(results)

output_file = RESULT_DIR / "analysis_results.csv"

df.to_csv(output_file, index=False)

print("================================")
print("ANALYSIS COMPLETED")
print("================================")

print(df.to_string(index=False))

print("\nResults saved to:", output_file)
