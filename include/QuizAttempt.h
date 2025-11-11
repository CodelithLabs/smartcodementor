#include "QuizAttempt.h"
#include <iostream>  // For std::cerr
#include <utility> // For std::move

QuizAttempt::QuizAttempt(const std::string& id, const std::string& qTitle, size_t totalQuestions)
    : studentId(id), quizTitle(qTitle), score(0) {
    
    correctness.resize(totalQuestions, false);
    userAnswers.resize(totalQuestions, -1); // -1 means "not answered"
}

void QuizAttempt::recordAnswer(const Question& question, size_t questionIndex, int answer) {
    // 1. Store the user's answer
    userAnswers[questionIndex] = answer;

    // 2. Calculate if it's correct
    bool isCorrect = question.isCorrect(answer);

    // 3. Update score and correctness
    if (isCorrect) {
        score++;
    }

    // Add bounds check for safety
    if (questionIndex < correctness.size()) {
        correctness[questionIndex] = isCorrect;
    } else {
        std::cerr << "Error: questionIndex " << questionIndex << " out of bounds." << std::endl;
    }
}

int QuizAttempt::getScore() const {
    return score;
}

size_t QuizAttempt::getTotalQuestions() const {
    return correctness.size();
}