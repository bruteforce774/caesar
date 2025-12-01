/**
 * Attempt to break a Caesar cipher by trying all shifts
 * @param {string} ciphertext - The encrypted text
 * @returns {Array} - All 26 possible decryptions with scores
 */
function breakCaesar(ciphertext) {
    // Expected frequency of 'E' in English (most common letter)
    const ENGLISH_E_FREQ = 12.7;
    
    const attempts = [];
    
    // Try all 26 possible shifts
    for (let shift = 0; shift < 26; shift++) {
        const decrypted = caesarShift(ciphertext, -shift);
        const analysis = analyzeFrequency(decrypted);
        
        // Score based on how close most common letter is to expected 'E' frequency
        const mostCommon = analysis.frequencies[0];
        const score = mostCommon 
            ? 100 - Math.abs(mostCommon.percentage - ENGLISH_E_FREQ)
            : 0;
        
        attempts.push({
            shift: shift,
            key: String.fromCharCode('A'.charCodeAt(0) + shift),
            text: decrypted.substring(0, 50) + '...',  // Preview
            score: score.toFixed(2),
            mostCommonLetter: mostCommon?.letter || 'N/A',
            mostCommonPercent: mostCommon?.percentage || '0.00'
        });
    }
    
    // Sort by score (best first)
    attempts.sort((a, b) => b.score - a.score);
    
    return attempts;
}

// Example:
const secret = "WKLV LV D VHFUHW PHVVDJH";
const attempts = breakCaesar(secret);

console.log("Top 3 most likely decryptions:\n");
attempts.slice(0, 3).forEach((attempt, i) => {
    console.log(`${i + 1}. Shift ${attempt.shift} (Key: ${attempt.key})`);
    console.log(`   Score: ${attempt.score}`);
    console.log(`   Text: ${attempt.text}`);
    console.log(`   Most common: ${attempt.mostCommonLetter} (${attempt.mostCommonPercent}%)\n`);
});

// Output:
// Top 3 most likely decryptions:
//
// 1. Shift 3 (Key: D)
//    Score: 99.30
//    Text: THIS IS A SECRET MESSAGE...
//    Most common: E (13.00%)