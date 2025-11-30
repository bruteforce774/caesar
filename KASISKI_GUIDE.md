# Kasiski Attack - Study Guide

## Overview

This program demonstrates how to **break** the Vigenère cipher using the techniques described in your cryptography passage. It's designed to be educational, with extensive comments explaining both the C++ code and the cryptographic concepts.

## How to Compile and Run

```bash
# Compile
g++ -std=c++17 -Wall kasiski_attack.cpp -o kasiski_attack

# Run with example ciphertext (built-in)
./kasiski_attack

# Run with a file
./kasiski_attack sample_cipher.txt

# Run with your own encrypted file
./kasiski_attack encrypted_message.txt
```

## What the Program Does

The program breaks a Vigenère cipher in three stages:

### 1. Kasiski Method - Finding Key Length
- Searches for repeated trigrams (3 letters) and tetragrams (4 letters)
- Calculates distances between repetitions
- Uses GCD (Greatest Common Divisor) to find likely key length

### 2. Index of Coincidence - Confirming Key Length
- Tests different key lengths statistically
- English text has IC ≈ 0.067
- Random text has IC ≈ 0.038
- The correct key length shows English-like IC values

### 3. Frequency Analysis - Recovering the Key
- Separates ciphertext into columns (based on key length)
- Each column is a Caesar cipher
- Uses chi-squared test to find the shift for each column
- Recovers the complete key letter by letter

## Key C++ Concepts to Study

### 1. **Data Structures**

**Maps for counting occurrences** (kasiski_attack.cpp:140-147):
```cpp
std::map<std::string, std::vector<int>> sequences;
```
Unlike your frequency analysis which uses arrays for 26 letters, here we use a `std::map` because:
- We don't know which trigrams will repeat
- The keys are strings ("PTR", "RYR"), not easily converted to array indices
- We need to store multiple positions for each sequence

**Vectors for dynamic lists** (throughout):
```cpp
std::vector<int> distances;
```
Vectors are like arrays but can grow dynamically. Used for storing positions and distances.

### 2. **Algorithms**

**Greatest Common Divisor (GCD)** (kasiski_attack.cpp:71-82):
```cpp
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;  // Same modulo operator as your ciphers!
        a = temp;
    }
    return a;
}
```
This is the Euclidean algorithm - uses the same `%` modulo you saw in Caesar and Vigenère!

**Example:**
- gcd(12, 18): 18 % 12 = 6, then 12 % 6 = 0, result = 6
- If distances are [6, 12, 36], GCD = 6 (likely key length!)

### 3. **Modulo Arithmetic - The Key Connection**

**Columnar separation** (kasiski_attack.cpp:391-397):
```cpp
std::vector<std::string> columns(key_length);

for (size_t i = 0; i < ciphertext.length(); i++) {
    columns[i % key_length] += ciphertext[i];
}
```

**This is EXACTLY the same modulo from your vigenere.cpp:25!**
- Your encryption: `key[key_pos % key.length()]` picks which key letter to use
- This attack: `columns[i % key_length]` separates letters encrypted by same key position
- It's the **inverse operation** - using modulo to reverse what modulo created!

### 4. **Statistical Analysis**

**Chi-squared test** (kasiski_attack.cpp:476-487):
```cpp
double chi_squared(const double observed[26], const double expected[26]) {
    double chi2 = 0.0;

    for (int i = 0; i < 26; i++) {
        if (expected[i] > 0) {
            double diff = observed[i] - expected[i];
            chi2 += (diff * diff) / expected[i];
        }
    }

    return chi2;
}
```

This measures "how English-like" the frequencies are:
- Lower chi-squared = better match to English
- Used to find the correct Caesar shift for each column

### 5. **Iterators and Range-based Loops**

**Modern C++ iteration** (kasiski_attack.cpp:146-154):
```cpp
for (const auto& [seq, positions] : tetragrams) {
    // seq is the string (e.g., "PTR")
    // positions is the vector of where it appears
}
```

This is **structured binding** (C++17):
- `auto` deduces the type automatically
- `&` makes it a reference (no copying)
- `const` means read-only
- `[seq, positions]` unpacks the map's key-value pairs

### 6. **String Manipulation**

**substring extraction** (kasiski_attack.cpp:145):
```cpp
std::string ngram = text.substr(i, n);
```
- `substr(start, length)` extracts a portion
- Example: "HELLO".substr(1, 3) = "ELL"

**String building** (kasiski_attack.cpp:395):
```cpp
columns[i % key_length] += ciphertext[i];
```
- The `+=` operator appends to a string
- Builds each column character by character

## Cryptographic Concepts to Study

### 1. **Why Repeated Sequences Occur**

From your vigenere.cpp, encryption works like this:
```
Position:  0 1 2 3 4 5 6 7 8 9 10 11 12 ...
Key:       D O G D O G D O G D O  G  D  ... (repeats every 3)
Plaintext: T H E C A T T H E D O  G  T  ...
```

If "THE" appears at positions 0 and 6:
- Position 0: encrypted with "DOG"
- Position 6: encrypted with "DOG" (key repeated!)
- **Same plaintext + same key = same ciphertext!**
- Distance = 6 - 0 = 6 (multiple of key length 3)

### 2. **Index of Coincidence - The Deep Insight**

English letter frequencies are uneven:
- 'E' appears ~13% of the time
- 'Z' appears ~0.07% of the time

**Probability two random English letters match:**
- Both 'E': 0.13 × 0.13 = 0.0169
- Both 'T': 0.09 × 0.09 = 0.0081
- ... add all 26 letters ...
- **Total ≈ 0.067 (about 1/15)**

**Probability two random letters match:**
- Each letter 1/26 = 0.0385
- All equal: 26 × (1/26)² = **1/26 ≈ 0.0385**

**When you split by correct key length:**
- Each column is Caesar cipher → still English → IC ≈ 0.067

**When you split by wrong key length:**
- Columns mix different key positions → random-like → IC ≈ 0.038

This is **brilliant** - you can detect the structure without knowing the key!

### 3. **Breaking Caesar by Frequency**

Each column is just a Caesar cipher. The program tries all 26 shifts:

```cpp
for (int shift = 0; shift < 26; shift++) {
    // Decrypt with this shift
    // Compare frequencies to English
    // Keep the best match
}
```

**Example:** If column has lots of 'X':
- Decrypt with shift 19: 'X' → 'E'
- Check if frequencies match English
- If yes, shift 19 is correct!
- So key letter = 'A' + 19 = 'T'

### 4. **The Complete Attack Flow**

```
1. Ciphertext: "ZVZPVTOGGEKHXSN..."
                    ↓
2. Find "YHCI" at positions 42, 78 → distance 36
                    ↓
3. Distance 36 suggests key length 6
                    ↓
4. Index of Coincidence confirms: IC highest at 6
                    ↓
5. Split into 6 columns:
   Column 0: Z T K Y V C ... (positions 0, 6, 12, ...)
   Column 1: V O H R I S ... (positions 1, 7, 13, ...)
   ...
                    ↓
6. Frequency analysis on each column:
   Column 0 most common: 'G' → shift from 'E' = 2 → key[0] = 'C'
   Column 1 most common: 'S' → shift from 'E' = 14 → key[1] = 'O'
   ...
                    ↓
7. Recovered key: "CRYPTO" (example)
                    ↓
8. Decrypt with vigenere_decrypt(ciphertext, "CRYPTO")
```

## Exercises to Deepen Understanding

### Exercise 1: Trace the Modulo
Pick a small example:
- Ciphertext: "ABCDEFGH" (length 8)
- Key length: 3

What goes in each column?
```
i=0: 0 % 3 = 0 → column[0] gets 'A'
i=1: 1 % 3 = 1 → column[1] gets 'B'
i=2: 2 % 3 = 2 → column[2] gets 'C'
i=3: 3 % 3 = 0 → column[0] gets 'D'
...
```

Result: column[0]="AD?", column[1]="BE?", column[2]="CF?"

### Exercise 2: Calculate IC by Hand
Take a short text: "AAB"
- Count: A=2, B=1, total=3
- Formula: Σ(fi × (fi-1)) / (N × (N-1))
- = (2×1 + 1×0) / (3×2)
- = 2 / 6 = 0.333

### Exercise 3: Understand GCD
Why does GCD find key length?
- Distances: [6, 12, 18]
- All are multiples of 6
- 6 is the largest number that divides all of them
- So key length is probably 6 (or a divisor like 2 or 3)

### Exercise 4: Modify the Code
Try changing the program to:
1. Only show tetragrams (more reliable than trigrams)
2. Test different IC thresholds (change 0.060 in line 424)
3. Add bigram (2-letter) detection

## Connection to Your Other Programs

**caesar.cpp** → Breaking one column
**vigenere.cpp** → What we're attacking
**freq_analysis.cpp** → Used in `calculate_frequencies()`
**kasiski_attack.cpp** → Combines all concepts!

The beauty is: **you already understand all the pieces!**
- Character arithmetic: `c - 'A'`
- Modulo wrapping: `% 26` and `% key_length`
- Arrays for counting: `int counts[26]`
- File I/O: `read_file()`
- References: `const std::string&`

This program just combines them in a new way.

## What Makes This Program Educational

1. **Extensive Comments**: Every function explains the "why", not just the "what"
2. **Real Cryptography**: This is how Vigenère was actually broken in 1863
3. **Multiple Approaches**: Shows Kasiski AND Index of Coincidence
4. **Builds on Your Code**: Uses patterns you already know
5. **Interactive**: You can test different key lengths and see results

## Further Study

After understanding this program, you could:

1. **Implement automatic key length detection** (no user input needed)
2. **Add support for different languages** (different frequency tables)
3. **Implement the "coincidence counting" method** (the sliding strips from the passage)
4. **Try breaking your own Vigenère-encrypted messages**
5. **Research modern ciphers** (AES, RSA) and why they resist these attacks

## Questions to Test Your Understanding

1. Why do we need `+ 26` in the modulo for decryption?
2. What's the difference between `std::map` and an array? When to use each?
3. Why are tetragrams more reliable than trigrams for Kasiski?
4. How does IC distinguish between correct and incorrect key lengths?
5. Why does chi-squared test work for finding the Caesar shift?
6. What would happen if the key length equals the message length?

## Key Takeaway

**This attack exploits the mathematical structure of the Vigenère cipher:**
- The repeating key creates patterns (Kasiski exploits this)
- Each column is simple Caesar (frequency analysis breaks this)
- Modular arithmetic connects everything (same `%` operator throughout)

The cipher that was "unbreakable" for 300 years falls to elementary math and statistics!

---

Happy studying! Work through the code slowly, trace examples by hand, and experiment with modifications. Understanding this will give you deep insight into both C++ and cryptography.
