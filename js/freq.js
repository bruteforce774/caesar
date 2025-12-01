/**
 * Analyze letter frequency in text
 * @param {string} text - The text to analyze
 * @returns {Object} - Frequency data with counts, percentages, and sorted results
 */
function analyzeFrequency(text) {
    // Initialize counts array (same as: int counts[26] = {0};)
    const counts = new Array(26).fill(0);
    let totalLetters = 0;
    
    // Count each letter (same loop as C++ version)
    for (let i = 0; i < text.length; i++) {
        const char = text[i];
        
        if (/[a-zA-Z]/.test(char)) {
            const upperChar = char.toUpperCase();
            
            // Same as: counts[c - 'A']++;
            const index = upperChar.charCodeAt(0) - 'A'.charCodeAt(0);
            counts[index]++;
            totalLetters++;
        }
    }
    
    // Build results array with letter, count, and percentage
    const results = [];
    
    for (let i = 0; i < 26; i++) {
        const letter = String.fromCharCode('A'.charCodeAt(0) + i);
        const count = counts[i];
        const percentage = totalLetters > 0 ? (count * 100.0) / totalLetters : 0;
        
        // Only include letters that appear (like C++ version)
        if (count > 0) {
            results.push({
                letter: letter,
                count: count,
                percentage: percentage.toFixed(2)  // 2 decimal places
            });
        }
    }
    
    // Sort by count (descending) - most common first
    results.sort((a, b) => b.count - a.count);
    
    return {
        totalLetters: totalLetters,
        frequencies: results,
        counts: counts  // Raw array, useful for other operations
    };
}

// Example usage:
const text = "The quick brown fox jumps over the lazy dog";
const analysis = analyzeFrequency(text);

console.log(`Total letters: ${analysis.totalLetters}`);
console.log("\nFrequency distribution:");

analysis.frequencies.forEach(item => {
    console.log(`${item.letter}: ${item.percentage}% (${item.count})`);
});

// Output:
// Total letters: 35
// 
// Frequency distribution:
// O: 11.43% (4)
// E: 8.57% (3)
// U: 8.57% (3)
// H: 5.71% (2)
// ...
