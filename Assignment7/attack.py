
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

BLOCK_SIZE = AES.block_size


def padding_oracle_attack(iv, ciphertext, oracle):
    """
    Recover plaintext using only the IV, ciphertext, and oracle.

    oracle must have the interface:
        oracle(iv, ciphertext) -> bool
    """
    if len(iv) != BLOCK_SIZE:
        raise ValueError("IV must be exactly 16 bytes.")

    if not ciphertext or len(ciphertext) % BLOCK_SIZE != 0:
        raise ValueError("Ciphertext must contain complete AES blocks.")

    query_count = 0

    def query(test_iv, test_ciphertext):
        nonlocal query_count
        query_count += 1
        return oracle(test_iv, test_ciphertext)

    ciphertext_blocks = [
        ciphertext[i:i + BLOCK_SIZE]
        for i in range(0, len(ciphertext), BLOCK_SIZE)
    ]

    recovered_blocks = []

    # Recover each plaintext block independently.
    for block_index, target_block in enumerate(ciphertext_blocks):

        if block_index == 0:
            original_previous = iv
        else:
            original_previous = ciphertext_blocks[block_index - 1]

        intermediate = bytearray(BLOCK_SIZE)
        crafted = bytearray(BLOCK_SIZE)

        # Recover the bytes from right to left.
        for position in range(BLOCK_SIZE - 1, -1, -1):
            padding_value = BLOCK_SIZE - position

            # Force already-recovered suffix bytes to the
            # desired padding value.
            for j in range(position + 1, BLOCK_SIZE):
                crafted[j] = intermediate[j] ^ padding_value

            found = False

            for candidate in range(256):
                crafted[position] = candidate ^ padding_value

                valid = query(bytes(crafted), target_block)

                if not valid:
                    continue

                # For padding length 1, reject accidental matches
                # that actually represent longer valid padding.
                if padding_value == 1:
                    verification = bytearray(crafted)
                    verification[position - 1] ^= 1

                    still_valid = query(
                        bytes(verification), target_block
                    )

                    if not still_valid:
                        continue

                intermediate[position] = candidate
                found = True
                break

            if not found:
                raise RuntimeError(
                    f"Could not recover block {block_index}, "
                    f"byte {position}."
                )

        # P_i = D_K(C_i) XOR C_(i-1)
        plaintext_block = bytes(
            intermediate[i] ^ original_previous[i]
            for i in range(BLOCK_SIZE)
        )

        recovered_blocks.append(plaintext_block)

    padded_plaintext = b"".join(recovered_blocks)

    try:
        plaintext = unpad(padded_plaintext, BLOCK_SIZE)
    except ValueError as exc:
        raise RuntimeError(
            "Recovered data has invalid PKCS#7 padding."
        ) from exc

    return plaintext, query_count