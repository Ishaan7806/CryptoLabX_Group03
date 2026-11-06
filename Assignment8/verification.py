
from sympy import isprime


def verify_prime(n, bit_length, rounds, bases):

    # Check actual bit length
    correct_bit_length = (n.bit_length() == bit_length)

    # Check that the number is odd
    is_odd = (n % 2 == 1)

    # Repeat Miller-Rabin with the SAME bases
    from miller_rabin import miller_rabin

    mr_result, verified_bases = miller_rabin(
        n,
        rounds=rounds,
        bases=bases
    )

    # Independent verification using SymPy
    independent_result = bool(isprime(n))

    return {
        "correct_bit_length": correct_bit_length,
        "is_odd": is_odd,
        "miller_rabin_passed": mr_result,
        "independent_verification": independent_result,
        "verified_bases": verified_bases
    }
