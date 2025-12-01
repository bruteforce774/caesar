/**
 * Caesar cipher encryption/decryption
 * @param {string} text - The text to process
 * @param {number} shift - The shift amount (-25 to 25)
 * @returns {string} - The shifted text (uppercase, non-letters preserved)
 */
function caesarShift(text, shift) {
    let result = '';
    
    for (let i = 0; i < text.length; i++) {
        let char = text[i];
        
        // Check if it's a letter (same as std::isalpha)
        if (/[a-zA-Z]/.test(char)) {
            // Convert to uppercase (same as std::toupper)
            char = char.toUpperCase();
            
            // Convert letter to position (0-25)
            // Same as: int pos = c - 'A';
            let pos = char.charCodeAt(0) - 'A'.charCodeAt(0);
            
            // Apply the shift with modulo wrapping
            // Same as: pos = (pos + shift + 26) % 26;
            pos = (pos + shift + 26) % 26;
            
            // Convert back to letter
            // Same as: c = pos + 'A';
            char = String.fromCharCode(pos + 'A'.charCodeAt(0));
        }
        
        result += char;
    }
    
    return result;
}

// Examples:
console.log(caesarShift("Hello, World!", 3));   // "KHOOR, ZRUOG!"
console.log(caesarShift("KHOOR, ZRUOG!", -3));  // "HELLO, WORLD!" (decrypt)
console.log(caesarShift("XYZ", 5));             // "CDE" (wraps around)
