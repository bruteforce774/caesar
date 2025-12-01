/**
 * Create a simple ASCII bar chart of frequencies
 * @param {string} text - The text to analyze
 * @returns {string} - ASCII bar chart
 */
function frequencyChart(text) {
    const analysis = analyzeFrequency(text);
    let chart = `Letter Frequencies (Total: ${analysis.totalLetters} letters)\n\n`;
    
    // Find max count for scaling
    const maxCount = Math.max(...analysis.frequencies.map(f => f.count));
    const maxBarLength = 50;  // Maximum bar width in characters
    
    analysis.frequencies.forEach(item => {
        // Scale bar length
        const barLength = Math.round((item.count / maxCount) * maxBarLength);
        const bar = '█'.repeat(barLength);
        
        chart += `${item.letter}: ${bar} ${item.count} (${item.percentage}%)\n`;
    });
    
    return chart;
}

// Example:
const encrypted = caesarShift("This is a secret message that has been encrypted", 5);
console.log(frequencyChart(encrypted));

// Output:
// Letter Frequencies (Total: 38 letters)
// 
// X: ██████████████████████████████████████████████████ 6 (15.79%)
// J: ████████████████████████████████████████████ 5 (13.16%)
// H: ███████████████████████████████████ 4 (10.53%)
// ...
