/**
 * Remove non-alphabetic characters and convert to uppercase
 * @param {string} text - The text to clean
 * @returns {string} - Cleaned text (only uppercase letters)
 */
function cleanText(text) {
    return text
        .toUpperCase()
        .replace(/[^A-Z]/g, '');  // Remove anything that's not A-Z
}

// Example:
console.log(cleanText("Hello, World! 123"));  // "HELLOWORLD"
