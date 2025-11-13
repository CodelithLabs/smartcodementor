#include "Question.h"
#include <utility> // For std::move

Question::Question(std::string txt, std::vector<std::string> opts, int correct)
    : text(std::move(txt)), options(std::move(opts)), correctAnswerIndex(correct) {}

const std::string& Question::getText() const {
    return text;
}

const std::vector<std::string>& Question::getOptions() const {
    return options;
}

bool Question::isCorrect(int answerIndex) const {
    return answerIndex == correctAnswerIndex;
}