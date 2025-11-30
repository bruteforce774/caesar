#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>

std::string caesar_encrypt(const std::string& text, int shift) {
    std::string result = text;
    
    for (char& c : result) {
        if (std::isalpha(c)) {
            c = std::toupper(c);
            int pos = c - 'A';
            pos = (pos + shift + 26) % 26;
            c = pos + 'A';
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
    buffer << file.rdbuf();  // Read entire file into buffer
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
    // Check argument count
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <filename> <shift>" << std::endl;
        return 1;
    }
    
    // Parse and validate shift
    int shift;
    try {
        shift = std::stoi(argv[2]);
        if (shift < -25 || shift > 25) {
            std::cerr << "Error: shift must be between -25 and 25" << std::endl;
            return 1;
        }
    } catch (...) {
        std::cerr << "Error: shift must be a valid integer" << std::endl;
        return 1;
    }
    
    // Get filenames
    std::string input_filename = argv[1];
    std::string output_filename = "shifted_" + input_filename;
    
    // Read, process, and write
    std::string content = read_file(input_filename);
    std::string processed = caesar_encrypt(content, shift);
    write_file(output_filename, processed);
    
    std::cout << "Processed " << input_filename << " with shift " << shift << std::endl;
    std::cout << "Output written to " << output_filename << std::endl;
    
    return 0;
}
