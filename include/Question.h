#pragma once

#include <string>
#include <vector>

class Question {
private:
    std::string text;
    std::vector<std::string> options;
    int correctAnswerIndex;

public:
    Question(std::string txt, std::vector<std::string> opts, int correct);

    const std::string& getText() const;
    const std::vector<std::string>& getOptions() const;
    bool isCorrect(int answerIndex) const;
};