
import csv

from prime_generator import generate_prime
from verification import verify_prime


def main():

    BIT_LENGTH = 512
    ROUNDS = 40

    print("=" * 65)
    print("     CRYPTOGRAPHIC PRIME GENERATION USING MILLER-RABIN")
    print("=" * 65)

    print("\nGenerating a probable prime...")
    print("Assigned bit length:", BIT_LENGTH)
    print("Miller-Rabin rounds:", ROUNDS)

    # Generate prime
    result = generate_prime(BIT_LENGTH, ROUNDS)

    prime = result["prime"]

    # Independent verification
    verification = verify_prime(
        prime,
        BIT_LENGTH,
        ROUNDS,
        result["bases"]
    )

    print("\n" + "=" * 65)
    print("EXPERIMENTAL RESULTS")
    print("=" * 65)

    print("\nParameter                         Value")
    print("-" * 65)

    print(f"Assigned bit length              {BIT_LENGTH}")
    print(f"Generated prime candidate        {prime}")
    print(f"Actual bit length                {result['bit_length']}")
    print(f"Number of candidates tested      {result['candidates_tested']}")
    print(f"Number of Miller-Rabin rounds    {result['rounds']}")
    print(f"Total MR rounds executed         {result['total_mr_rounds']}")
    print(f"Total execution time             {result['execution_time']:.6f} seconds")

    print("\n" + "=" * 65)
    print("VERIFICATION")
    print("=" * 65)

    print("Correct bit length:",
          verification["correct_bit_length"])

    print("Number is odd:",
          verification["is_odd"])

    print("Miller-Rabin verification:",
          verification["miller_rabin_passed"])

    print("Independent SymPy verification:",
          verification["independent_verification"])

    print("\nBases used for final candidate:")

    for i, base in enumerate(result["bases"], start=1):
        print(f"Round {i}: {base}")

    # Save experimental results
    with open("experimental_results.csv", "w", newline="") as file:

        writer = csv.writer(file)

        writer.writerow(["Parameter", "Value"])

        writer.writerow(["Assigned bit length", BIT_LENGTH])
        writer.writerow(["Generated prime candidate", prime])
        writer.writerow(["Actual bit length", result["bit_length"]])
        writer.writerow(["Candidates tested", result["candidates_tested"]])
        writer.writerow(["Miller-Rabin rounds", ROUNDS])
        writer.writerow(["Total MR rounds executed",
                         result["total_mr_rounds"]])
        writer.writerow(["Execution time (seconds)",
                         result["execution_time"]])

        writer.writerow(["Final candidate MR bases",
                         ";".join(map(str, result["bases"]))])

        writer.writerow(["Independent verification",
                         verification["independent_verification"]])

    print("\nExperimental results saved to experimental_results.csv")

    if (
        verification["correct_bit_length"]
        and verification["is_odd"]
        and verification["miller_rabin_passed"]
        and verification["independent_verification"]
    ):
        print("\nSUCCESS: Probable prime generated and verified!")

    else:
        print("\nVERIFICATION FAILED!")


if __name__ == "__main__":
    main()
