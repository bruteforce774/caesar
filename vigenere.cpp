#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

std::string vigenere_encrypt(const std::string& text, const std::string& key) {
    std::string result = text;
    int key_pos = 0;  // Track position in key (only advances for letters)
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            c = std::toupper(c);
            
            // Get shift from current key letter
            char key_letter = std::toupper(key[key_pos % key.length()]);
            int shift = key_letter - 'A';
            
            // Apply shift (same as Caesar)
            int pos = c - 'A';
            pos = (pos + shift) % 26;
            c = pos + 'A';
            
            key_pos++;  // Only advance key position for letters
        }
    }
    
    return result;
}

std::string vigenere_decrypt(const std::string& text, const std::string& key) {
    std::string result = text;
    int key_pos = 0;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            c = std::toupper(c);
            
            // Get shift from current key letter
            char key_letter = std::toupper(key[key_pos % key.length()]);
            int shift = key_letter - 'A';
            
            // Apply negative shift for decryption
            int pos = c - 'A';
            pos = (pos - shift + 26) % 26;  // Note: minus shift, plus 26 for wrapping
            c = pos + 'A';
            
            key_pos++;
        }
    }
    
    return result;
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
    // Check argument count: need program name, mode, filename, key
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <encrypt|decrypt> <filename> <key>" << std::endl;
        return 1;
    }
    
    std::string mode = argv[1];
    std::string input_filename = argv[2];
    std::string key = argv[3];
    
    // Validate key is not empty
    if (key.empty()) {
        std::cerr << "Error: key cannot be empty" << std::endl;
        return 1;
    }
    
    // Validate key contains only letters
    for (char c : key) {
        if (!std::isalpha(c)) {
            std::cerr << "Error: key must contain only letters" << std::endl;
            return 1;
        }
    }
    
    // Read input file
    std::string content = read_file(input_filename);
    
    // Process based on mode
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
    
    // Write output
    std::string output_filename = output_prefix + input_filename;
    write_file(output_filename, processed);
    
    std::cout << "Processed " << input_filename << " in " << mode << " mode with key '" 
              << key << "'" << std::endl;
    std::cout << "Output written to " << output_filename << std::endl;
    
    return 0;
}
