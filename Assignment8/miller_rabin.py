import secrets


def miller_rabin(n, rounds=40, bases=None):
    """
    Miller-Rabin probabilistic primality test.

    Returns:
        is_prime: True if n passes all rounds.
        tested_bases: Bases actually tested.
    """

    if n < 2:
        return False, []

    if n in (2, 3):
        return True, []

    if n % 2 == 0:
        return False, []

    # Write n - 1 = d * 2^s
    d = n - 1
    s = 0

    while d % 2 == 0:
        d //= 2
        s += 1

    if bases is not None and len(bases) != rounds:
        raise ValueError("Number of bases must equal rounds")

    tested_bases = []

    for i in range(rounds):

        # Generate a random base between 2 and n-2
        if bases is None:
            a = 2 + secrets.randbelow(n - 3)
        else:
            a = bases[i]

        if not 2 <= a <= n - 2:
            raise ValueError("Base must be between 2 and n-2")

        tested_bases.append(a)

        # Calculate a^d mod n
        x = pow(a, d, n)

        if x == 1 or x == n - 1:
            continue

        composite = True

        for _ in range(s - 1):
            x = pow(x, 2, n)

            if x == n - 1:
                composite = False
                break

        if composite:
            return False, tested_bases

    return True, tested_bases