# Monoalphabetic Substitution Cipher & Cryptanalysis

## Objective

Implementation and cryptanalysis of a **Monoalphabetic Substitution Cipher** using **C++**.

The plaintext is a one-page excerpt from *Modern Cryptography* by Katz and Lindell, as assigned to the group.

## Features

* Monoalphabetic substitution encryption
* Ciphertext generation
* Letter frequency analysis
* Word frequency analysis
* One-, two-, and three-letter word analysis
* Repeated word detection
* Repeated-letter pattern analysis
* Iterative substitution and partial plaintext recovery
* Substitution key recovery
* Re-encryption-based solution verification

## Main Functions

```text
frequency_analysis()
word_frequency_analysis()
pattern_analysis()
apply_substitution()
display_partial_plaintext()
verify_solution()
```

## Project Structure

```text
├── main.cpp
├── README.md
├── input/
│   └── plaintext.txt
└── output/
    ├── ciphertext.txt
    └── recovered_plaintext.txt
```

## How to Run

Compile:

```bash
g++ main.cpp -o cipher
```

Run:

```bash
./cipher
```

For Windows:

```powershell
g++ main.cpp -o cipher.exe
.\cipher.exe
```

## Cryptanalysis Approach

The ciphertext is analyzed using letter frequencies, word lengths, repeated words, and repeated-letter patterns. Candidate substitutions are tested iteratively by examining the resulting partial plaintext. Incorrect hypotheses are rejected until meaningful plaintext is recovered.

Finally, the recovered key is validated by re-encrypting the plaintext and comparing it with the original ciphertext.

## Language

**C++**
