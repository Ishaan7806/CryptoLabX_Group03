import secrets
import time

from miller_rabin import miller_rabin


def generate_prime(bit_length=512, rounds=40):

    if bit_length < 2:
        raise ValueError("Bit length must be at least 2")

    candidates_tested = 0
    total_mr_rounds = 0

    start_time = time.perf_counter()

    while True:

        # Generate a random candidate
        candidate = secrets.randbits(bit_length)

        # Ensure exactly the required bit length
        candidate |= (1 << (bit_length - 1))

        # Ensure the candidate is odd
        candidate |= 1

        candidates_tested += 1

        # Apply Miller-Rabin
        is_prime, tested_bases = miller_rabin(
            candidate, rounds
        )

        total_mr_rounds += len(tested_bases)

        if is_prime:

            execution_time = time.perf_counter() - start_time

            return {
                "prime": candidate,
                "bit_length": candidate.bit_length(),
                "candidates_tested": candidates_tested,
                "rounds": rounds,
                "total_mr_rounds": total_mr_rounds,
                "bases": tested_bases,
                "execution_time": execution_time
            }