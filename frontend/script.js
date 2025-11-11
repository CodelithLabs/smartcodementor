document.addEventListener('DOMContentLoaded', () => {
    const runBtn = document.getElementById('run-btn');
    const hintBtn = document.getElementById('hint-btn');
    const codeEditor = document.getElementById('code-editor');
    const outputArea = document.getElementById('output-area');

    // Placeholder for C++ code
    codeEditor.value = `#include <iostream>

int main() {
    std::cout << "Hello, SmartCodeMentor!" << std::endl;
    return 0;
}`;

    runBtn.addEventListener('click', () => {
        const userCode = codeEditor.value;
        
        // --- TODO: Replace this with a real backend call ---
        // For now, we'll simulate a successful run.
        outputArea.style.color = '#9ece6a'; // Green for success
        outputArea.textContent = `Simulating execution of your code...\n\n> g++ your_code.cpp -o a.out\n> ./a.out\n\nHello, SmartCodeMentor!\n\nProcess finished with exit code 0.`;
        
        console.log("Running code:", userCode);
    });

    hintBtn.addEventListener('click', () => {
        // --- TODO: Replace this with a real AI backend call ---
        outputArea.style.color = '#ff9e64'; // Orange for hint
        outputArea.textContent = `💡 Hint: Try declaring a variable to store a number, like 'int myNumber = 42;', and then print it using 'std::cout << myNumber << std::endl;'.`;
        
        console.log("Getting a hint.");
    });
});
