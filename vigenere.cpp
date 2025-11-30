#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

// Internal function that performs Vigenère encryption or decryption
// Parameters:
//   text: the message to process
//   key: the keyword (e.g., "CAB")
//   direction: +1 for encryption, -1 for decryption
// Returns: the processed text (all uppercase, non-letters unchanged)
std::string vigenere_process(const std::string& text, const std::string& key, int direction) {
    std::string result = text;
    int key_pos = 0;  // Tracks position in key, only advances for letters
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            // Step 1: Convert plaintext letter to uppercase
            c = std::toupper(c);
            
            // Step 2: Get the corresponding key letter for this position
            // Use modulo to wrap around if key is shorter than text
            // Example: key "CAB" (length 3), position 5 → key[5 % 3] = key[2] = 'B'
            char key_letter = std::toupper(key[key_pos % key.length()]);
            
            // Step 3: Convert key letter to a shift amount (0-25)
            // 'A' → 0, 'B' → 1, 'C' → 2, etc.
            int shift = key_letter - 'A';
            
            // Step 4: Convert plaintext letter to position (0-25)
            // 'A' → 0, 'B' → 1, ..., 'Z' → 25
            int pos = c - 'A';
            
            // Step 5: Apply the cipher transformation
            // - For encryption (direction = 1): add the shift
            //   Example: 'S'(18) + shift 2 = 20 → 'U'
            // - For decryption (direction = -1): subtract the shift
            //   Example: 'U'(20) - shift 2 = 18 → 'S'
            // - The +26 ensures we never get negative numbers before modulo
            //   Example: 'B'(1) - shift 3 = -2, but (-2 + 26) % 26 = 24 → 'Y'
            // - The % 26 wraps around the alphabet
            //   Example: 'Y'(24) + shift 3 = 27, but 27 % 26 = 1 → 'B'
            pos = (pos + direction * shift + 26) % 26;
            
            // Step 6: Convert position back to a letter
            // 0 → 'A', 1 → 'B', etc.
            c = pos + 'A';
            
            // Step 7: Move to next position in key (only for letters)
            key_pos++;
        }
        // Non-alphabetic characters (spaces, punctuation) are left unchanged
        // and do NOT advance the key position
    }
    
    return result;
}

// Public wrapper for encryption
// Calls vigenere_process with direction = +1
std::string vigenere_encrypt(const std::string& text, const std::string& key) {
    return vigenere_process(text, key, 1);
}

// Public wrapper for decryption
// Calls vigenere_process with direction = -1
std::string vigenere_decrypt(const std::string& text, const std::string& key) {
    return vigenere_process(text, key, -1);
}

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

void write_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not write to file " << filename << std::endl;
        exit(1);
    }
    
    file << content;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <encrypt|decrypt> <filename> <key>" << std::endl;
        return 1;
    }
    
    std::string mode = argv[1];
    std::string input_filename = argv[2];
    std::string key = argv[3];
    
    if (key.empty()) {
        std::cerr << "Error: key cannot be empty" << std::endl;
        return 1;
    }
    
    for (char c : key) {
        if (!std::isalpha(c)) {
            std::cerr << "Error: key must contain only letters" << std::endl;
            return 1;
        }
    }
    
    std::string content = read_file(input_filename);
    
    std::string processed;
    std::string output_prefix;
    
    if (mode == "encrypt") {
        processed = vigenere_encrypt(content, key);
        output_prefix = "encrypted_";
    } else if (mode == "decrypt") {
        processed = vigenere_decrypt(content, key);
        output_prefix = "decrypted_";
    } else {
        std::cerr << "Error: mode must be 'encrypt' or 'decrypt'" << std::endl;
        return 1;
    }
    
    std::string output_filename = output_prefix + input_filename;
    write_file(output_filename, processed);
    
    std::cout << "Processed " << input_filename << " in " << mode << " mode with key '" 
              << key << "'" << std::endl;
    std::cout << "Output written to " << output_filename << std::endl;
    
    return 0;
}
