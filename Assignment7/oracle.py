from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import secrets

BLOCK_SIZE = 16

# Private to the simulated encryption/oracle service.
_KEY = secrets.token_bytes(BLOCK_SIZE)

# Demo message used only to prepare the challenge.
_DEMO_MESSAGE = b"Padding oracle attacks demonstrate a serious CBC vulnerability."


def create_challenge():
    """Return an IV and ciphertext, without exposing the AES key."""
    iv = secrets.token_bytes(BLOCK_SIZE)

    cipher = AES.new(_KEY, AES.MODE_CBC, iv)
    ciphertext = cipher.encrypt(pad(_DEMO_MESSAGE, BLOCK_SIZE))

    return iv, ciphertext


def has_valid_pkcs7_padding(data):
    """Check PKCS#7 padding without returning the plaintext."""
    if not data or len(data) % BLOCK_SIZE != 0:
        return False

    padding_length = data[-1]

    if padding_length < 1 or padding_length > BLOCK_SIZE:
        return False

    expected_padding = bytes([padding_length]) * padding_length
    return data[-padding_length:] == expected_padding


def padding_oracle(iv, ciphertext):
    """
    Return True if decrypted data has valid PKCS#7 padding.
    Return False otherwise.

    This deliberately vulnerable oracle is for the local lab only.
    """
    if len(iv) != BLOCK_SIZE:
        return False

    if not ciphertext or len(ciphertext) % BLOCK_SIZE != 0:
        return False

    try:
        cipher = AES.new(_KEY, AES.MODE_CBC, iv)
        plaintext = cipher.decrypt(ciphertext)

        return has_valid_pkcs7_padding(plaintext)

    except (ValueError, TypeError):
        return False