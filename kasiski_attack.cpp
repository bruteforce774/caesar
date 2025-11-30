// ============================================================================
// KASISKI ATTACK - Breaking the Vigenère Cipher
// ============================================================================
// This program demonstrates how to break a Vigenère cipher using:
// 1. Kasiski Method - finding repeated sequences to determine key length
// 2. Index of Coincidence - statistical method to confirm key length
// 3. Frequency Analysis - analyzing each column to recover the key
//
// The Vigenère cipher was considered unbreakable for 300 years until
// Kasiski published this method in 1863.
// ============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iomanip>
#include <cmath>


// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Reads entire file into a string
// Same pattern as your other cipher programs

std::string read_file(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


// Removes all non-alphabetic characters and converts to uppercase
// Ciphertext should be clean for analysis

std::string clean_text(const std::string& text) {
    std::string result;

    for (char c : text) {
        if (std::isalpha(c)) {
            result += std::toupper(c);
        }
    }

    return result;
}


// Calculate Greatest Common Divisor using Euclidean algorithm
// Used to find common factors among distances
// Example: gcd(12, 18) = 6

int gcd(int a, int b) {
    // Euclidean algorithm: gcd(a,b) = gcd(b, a mod b)
    // Keep dividing until remainder is 0

    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}


// Calculate GCD of a vector of numbers
// Finds the largest number that divides all distances

int gcd_of_vector(const std::vector<int>& numbers) {
    if (numbers.empty()) return 1;

    int result = numbers[0];

    for (size_t i = 1; i < numbers.size(); i++) {
        result = gcd(result, numbers[i]);
    }

    return result;
}


// ============================================================================
// KASISKI METHOD - Finding Repeated Sequences
// ============================================================================

// Structure to hold information about repeated sequences

struct Repetition {
    std::string sequence;           // The repeated n-gram (e.g., "PTR")
    std::vector<int> positions;     // Where it appears in ciphertext
    std::vector<int> distances;     // Distances between occurrences
};


// Find all repeated n-grams of a given length
// n = 3 for trigrams, n = 4 for tetragrams
// Returns a map: sequence -> positions where it appears

std::map<std::string, std::vector<int>> find_repeated_sequences(
    const std::string& text,
    int n
) {
    std::map<std::string, std::vector<int>> sequences;

    // Extract all n-grams and record their positions
    // Example: "HELLO" with n=3 gives "HEL" at 0, "ELL" at 1, "LLO" at 2

    for (size_t i = 0; i <= text.length() - n; i++) {
        std::string ngram = text.substr(i, n);
        sequences[ngram].push_back(i);
    }


    // Remove sequences that only appear once (not repeated)
    // We only care about repetitions!

    auto it = sequences.begin();
    while (it != sequences.end()) {
        if (it->second.size() < 2) {
            it = sequences.erase(it);  // Remove non-repeated sequences
        } else {
            ++it;
        }
    }

    return sequences;
}


// Calculate distances between all pairs of repetitions
// If a sequence appears at positions [5, 12, 33]:
//   Distance 12-5 = 7
//   Distance 33-5 = 28
//   Distance 33-12 = 21

std::vector<int> calculate_distances(const std::vector<int>& positions) {
    std::vector<int> distances;

    // For each pair of positions, calculate the distance
    // Number of pairs = N*(N-1)/2

    for (size_t i = 0; i < positions.size(); i++) {
        for (size_t j = i + 1; j < positions.size(); j++) {
            distances.push_back(positions[j] - positions[i]);
        }
    }

    return distances;
}


// Analyze repeated sequences using Kasiski method
// Longer sequences (tetragrams) are more reliable than trigrams

void kasiski_analysis(const std::string& ciphertext) {
    std::cout << "\n========================================\n";
    std::cout << "KASISKI METHOD - Repeated Sequences\n";
    std::cout << "========================================\n\n";


    // Look for tetragrams (4-letter sequences) - most reliable

    std::cout << "Looking for repeated TETRAGRAMS (4 letters):\n";
    std::cout << "--------------------------------------------\n";

    auto tetragrams = find_repeated_sequences(ciphertext, 4);

    std::vector<int> all_distances_4;

    for (const auto& [seq, positions] : tetragrams) {
        auto distances = calculate_distances(positions);

        std::cout << "\"" << seq << "\" at positions: ";
        for (int pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << " -> distances: ";
        for (int dist : distances) {
            std::cout << dist << " ";
            all_distances_4.push_back(dist);
        }
        std::cout << "\n";
    }


    // Look for trigrams (3-letter sequences) - more common but less reliable

    std::cout << "\nLooking for repeated TRIGRAMS (3 letters):\n";
    std::cout << "-------------------------------------------\n";

    auto trigrams = find_repeated_sequences(ciphertext, 3);

    std::vector<int> all_distances_3;

    // Limit output to avoid spam - only show first 10
    int count = 0;
    for (const auto& [seq, positions] : trigrams) {
        if (count++ >= 10) {
            std::cout << "... (showing first 10 trigrams)\n";
            break;
        }

        auto distances = calculate_distances(positions);

        std::cout << "\"" << seq << "\" at positions: ";
        for (int pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << " -> distances: ";
        for (int dist : distances) {
            std::cout << dist << " ";
            all_distances_3.push_back(dist);
        }
        std::cout << "\n";
    }


    // Analyze the distances to find likely key length

    std::cout << "\nAnalyzing distances:\n";
    std::cout << "--------------------\n";

    if (!all_distances_4.empty()) {
        int gcd_result = gcd_of_vector(all_distances_4);
        std::cout << "GCD of tetragram distances: " << gcd_result << "\n";
    }

    if (!all_distances_3.empty()) {
        int gcd_result = gcd_of_vector(all_distances_3);
        std::cout << "GCD of trigram distances: " << gcd_result << "\n";
    }


    // Count frequency of each distance to find common factors

    std::map<int, int> distance_freq;
    for (int d : all_distances_3) {
        distance_freq[d]++;
    }
    for (int d : all_distances_4) {
        distance_freq[d]++;
    }

    std::cout << "\nMost common distances:\n";

    // Convert map to vector for sorting
    std::vector<std::pair<int, int>> sorted_distances(distance_freq.begin(), distance_freq.end());
    std::sort(sorted_distances.begin(), sorted_distances.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    // Show top 10
    for (size_t i = 0; i < std::min(size_t(10), sorted_distances.size()); i++) {
        std::cout << "  Distance " << sorted_distances[i].first
                  << " appears " << sorted_distances[i].second << " times\n";
    }
}


// ============================================================================
// INDEX OF COINCIDENCE - Statistical Analysis
// ============================================================================

// Calculate Index of Coincidence for a string
// IC measures how "non-random" text is
//
// For English text: IC ≈ 0.0667 (about 1/15)
// For random text:  IC ≈ 0.0385 (about 1/26)
//
// Formula: IC = Σ(fi * (fi-1)) / (N * (N-1))
// where fi = frequency of letter i, N = total letters

double calculate_ic(const std::string& text) {
    if (text.length() < 2) return 0.0;


    // Count letter frequencies (same pattern as your freq_analysis.cpp)

    int counts[26] = {0};
    int total = 0;

    for (char c : text) {
        if (std::isalpha(c)) {
            counts[std::toupper(c) - 'A']++;
            total++;
        }
    }


    // Calculate IC using the formula
    // We're calculating: probability that two random letters are the same

    double sum = 0.0;
    for (int i = 0; i < 26; i++) {
        // For each letter, how many ways can we pick 2 of them?
        // That's fi * (fi - 1), or "fi choose 2"
        sum += counts[i] * (counts[i] - 1);
    }


    // Total ways to pick 2 letters from N letters is N * (N-1)

    double ic = sum / (total * (total - 1));

    return ic;
}


// Test different key lengths using Index of Coincidence
// The correct key length will show IC ≈ 0.067 (English-like)
// Wrong key lengths will show IC ≈ 0.038 (random)

void test_key_lengths_ic(const std::string& ciphertext, int max_length) {
    std::cout << "\n========================================\n";
    std::cout << "INDEX OF COINCIDENCE - Key Length Test\n";
    std::cout << "========================================\n\n";

    std::cout << "Testing key lengths 1-" << max_length << ":\n";
    std::cout << "(English text IC ≈ 0.067, random text IC ≈ 0.038)\n\n";

    std::vector<std::pair<int, double>> ic_scores;

    for (int key_len = 1; key_len <= max_length; key_len++) {

        // Split ciphertext into columns based on this key length
        // This is the "columnar organization" you mentioned!

        std::vector<std::string> columns(key_len);

        for (size_t i = 0; i < ciphertext.length(); i++) {
            columns[i % key_len] += ciphertext[i];  // Same modulo as vigenere!
        }


        // Calculate average IC across all columns
        // If key_len is correct, each column is a Caesar cipher (English-like)
        // If key_len is wrong, columns are mixed up (random-like)

        double total_ic = 0.0;

        for (int i = 0; i < key_len; i++) {
            total_ic += calculate_ic(columns[i]);
        }

        double avg_ic = total_ic / key_len;
        ic_scores.push_back({key_len, avg_ic});


        // Print results with visual indicator

        std::cout << "Key length " << std::setw(2) << key_len
                  << ": IC = " << std::fixed << std::setprecision(4) << avg_ic;

        if (avg_ic > 0.060) {
            std::cout << " *** LIKELY ***";
        }

        std::cout << "\n";
    }


    // Find the best candidate

    auto best = std::max_element(ic_scores.begin(), ic_scores.end(),
                                  [](const auto& a, const auto& b) {
                                      return a.second < b.second;
                                  });

    std::cout << "\nBest candidate: key length " << best->first
              << " with IC = " << best->second << "\n";
}


// ============================================================================
// FREQUENCY ANALYSIS - Breaking Each Column
// ============================================================================

// Expected letter frequencies in English (as percentages)
// E is most common at 13%, Z is least common at 0.07%

const double ENGLISH_FREQ[26] = {
    8.2, 1.5, 2.8, 4.3, 13.0, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074
};


// Calculate letter frequencies for a string
// Returns array of percentages (0-100)

void calculate_frequencies(const std::string& text, double freq[26]) {
    int counts[26] = {0};
    int total = 0;

    for (char c : text) {
        if (std::isalpha(c)) {
            counts[std::toupper(c) - 'A']++;
            total++;
        }
    }

    for (int i = 0; i < 26; i++) {
        freq[i] = total > 0 ? (counts[i] * 100.0) / total : 0.0;
    }
}


// Calculate chi-squared statistic
// Measures how well observed frequencies match expected frequencies
// Lower chi-squared = better match to English

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


// Try all 26 possible Caesar shifts on a string
// Find the shift that makes the frequency distribution most English-like
// This recovers one letter of the Vigenère key!

char break_caesar_shift(const std::string& column) {
    double best_chi2 = 999999.0;
    char best_shift = 'A';


    // Try each possible shift (0-25, or 'A'-'Z')

    for (int shift = 0; shift < 26; shift++) {

        // Decrypt this column with this shift
        // (Reverse the Caesar cipher)

        std::string decrypted;

        for (char c : column) {
            if (std::isalpha(c)) {
                int pos = std::toupper(c) - 'A';
                // Subtract the shift (with +26 to handle negatives)
                pos = (pos - shift + 26) % 26;
                decrypted += ('A' + pos);
            }
        }


        // Calculate frequency distribution of decrypted text

        double freq[26];
        calculate_frequencies(decrypted, freq);


        // Compare to English frequencies using chi-squared

        double chi2 = chi_squared(freq, ENGLISH_FREQ);


        // Keep track of best match

        if (chi2 < best_chi2) {
            best_chi2 = chi2;
            best_shift = 'A' + shift;
        }
    }

    return best_shift;
}


// ============================================================================
// KEY RECOVERY - Putting It All Together
// ============================================================================

// Recover the Vigenère key using frequency analysis on each column

std::string recover_key(const std::string& ciphertext, int key_length) {
    std::cout << "\n========================================\n";
    std::cout << "KEY RECOVERY - Frequency Analysis\n";
    std::cout << "========================================\n\n";

    std::cout << "Attempting to recover key of length " << key_length << "...\n\n";


    // Step 1: Separate ciphertext into columns
    // This is the columnar organization technique!

    std::vector<std::string> columns(key_length);

    for (size_t i = 0; i < ciphertext.length(); i++) {
        columns[i % key_length] += ciphertext[i];
    }


    // Step 2: Break each column as a Caesar cipher

    std::string recovered_key;

    for (int i = 0; i < key_length; i++) {
        std::cout << "Column " << i << " (positions " << i << ", "
                  << (i + key_length) << ", " << (i + 2*key_length)
                  << ", ...) has " << columns[i].length() << " letters\n";

        char key_letter = break_caesar_shift(columns[i]);
        recovered_key += key_letter;

        std::cout << "  -> Key letter " << i << " is: " << key_letter << "\n\n";
    }

    std::cout << "Recovered key: " << recovered_key << "\n";

    return recovered_key;
}


// ============================================================================
// DECRYPTION - Using the Recovered Key
// ============================================================================

// Decrypt Vigenère cipher (same algorithm as your vigenere.cpp)

std::string vigenere_decrypt(const std::string& text, const std::string& key) {
    std::string result = text;
    int key_pos = 0;

    for (char& c : result) {
        if (std::isalpha(c)) {
            c = std::toupper(c);

            char key_letter = std::toupper(key[key_pos % key.length()]);
            int shift = key_letter - 'A';

            int pos = c - 'A';
            pos = (pos - shift + 26) % 26;  // Subtract for decryption
            c = pos + 'A';

            key_pos++;
        }
    }

    return result;
}


// ============================================================================
// MAIN PROGRAM
// ============================================================================

int main(int argc, char* argv[]) {
    std::cout << "========================================\n";
    std::cout << "KASISKI ATTACK - Vigenère Cipher Breaker\n";
    std::cout << "========================================\n";


    // Get ciphertext from file or use example

    std::string ciphertext;

    if (argc > 1) {
        ciphertext = read_file(argv[1]);
        std::cout << "Loaded ciphertext from: " << argv[1] << "\n";
    } else {
        // Example ciphertext from the passage (key length 6)
        ciphertext = "ZVZPV TOGGE KHXSN LRYRP ZHZIO RZHZA ZCOAF PNOHF "
                     "VEYHC ILCVS MGRYR SYXYR YSIEK RGBYX YRRCR IIVYH "
                     "CIYBA GZSWE KDMIJ RTHVX ZIKG";
        std::cout << "Using example ciphertext from the passage.\n";
    }


    // Clean the ciphertext (remove spaces, convert to uppercase)

    ciphertext = clean_text(ciphertext);

    std::cout << "Ciphertext length: " << ciphertext.length() << " letters\n";
    std::cout << "Ciphertext: " << ciphertext << "\n";


    // Step 1: Kasiski analysis - find repeated sequences

    kasiski_analysis(ciphertext);


    // Step 2: Index of Coincidence - confirm key length

    test_key_lengths_ic(ciphertext, 15);


    // Step 3: Ask user for key length (or auto-detect)

    std::cout << "\n========================================\n";
    std::cout << "Based on the analysis above, what key length do you want to try?\n";
    std::cout << "Enter key length (or 0 to exit): ";

    int key_length;
    std::cin >> key_length;

    if (key_length <= 0 || key_length > 50) {
        std::cout << "Exiting.\n";
        return 0;
    }


    // Step 4: Recover the key using frequency analysis

    std::string recovered_key = recover_key(ciphertext, key_length);


    // Step 5: Decrypt using the recovered key

    std::cout << "\n========================================\n";
    std::cout << "DECRYPTION\n";
    std::cout << "========================================\n\n";

    std::string plaintext = vigenere_decrypt(ciphertext, recovered_key);

    std::cout << "Decrypted text:\n";
    std::cout << plaintext << "\n\n";


    // Optional: Try manual key entry if automatic recovery failed

    std::cout << "If the above doesn't look right, you can try a different key.\n";
    std::cout << "Enter key manually (or press Enter to finish): ";

    std::cin.ignore();  // Clear the newline from previous input
    std::string manual_key;
    std::getline(std::cin, manual_key);

    if (!manual_key.empty()) {
        plaintext = vigenere_decrypt(ciphertext, manual_key);
        std::cout << "\nDecrypted with key \"" << manual_key << "\":\n";
        std::cout << plaintext << "\n";
    }

    return 0;
}
