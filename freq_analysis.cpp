#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>

std::string read_from_stdin() {
    std::stringstream buffer;
    buffer << std::cin.rdbuf();
    return buffer.str();
}

std::string read_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        exit(1);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void analyze_frequency(const std::string& text) {
    int counts[26] = {0};
    int total_letters = 0;
    
    // Count each letter
    for (char c : text) {
        if (std::isalpha(c)) {
            c = std::toupper(c);
            counts[c - 'A']++;
            total_letters++;
        }
    }
    
    if (total_letters == 0) {
        std::cout << "No letters found in input." << std::endl;
        return;
    }
    
    // Print frequencies
    std::cout << "Letter frequencies (total letters: " << total_letters << "):\n\n";
    for (int i = 0; i < 26; i++) {
        char letter = 'A' + i;
        double percentage = (counts[i] * 100.0) / total_letters;
        if (counts[i] > 0) {  // Only show letters that appear
            std::cout << letter << ": " << std::setw(6) << std::fixed 
                      << std::setprecision(2) << percentage << "% (" 
                      << counts[i] << ")" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    std::string text;
    
    if (argc > 1) {
        // Read from file
        text = read_from_file(argv[1]);
    } else {
        // Read from stdin
        text = read_from_stdin();
    }
    
    analyze_frequency(text);
    
    return 0;
}
