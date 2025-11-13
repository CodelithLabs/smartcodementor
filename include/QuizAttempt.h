#pragma once

#include <string>
#include <vector>
#include <cstddef>     // <-- FIX: Added for size_t
#include "Question.h"  // Include dependencies

class QuizAttempt {
private:
    std::string studentId;
    std::string quizTitle;
    int score;
    std::vector<bool> correctness;
    std::vector<int> userAnswers;

public:
    // --- Declarations Only ---
    // (Notice the semicolons ';' and no function bodies)
    QuizAttempt(const std::string& id, const std::string& qTitle, size_t totalQuestions);

    void recordAnswer(const Question& question, size_t questionIndex, int answer);
    
    int getScore() const;
    
    size_t getTotalQuestions() const;
};