#include "Quiz.h"
#include <utility> // For std::move

Quiz::Quiz(const std::string& newTitle, const std::string& newCreator)
    : title(newTitle), creator(newCreator) {
    // Constructor body
}

void Quiz::addQuestion(const Question& question) {
    questions.push_back(question);
}

const std::string& Quiz::getTitle() const {
    return title;
}

const std::vector<Question>& Quiz::getQuestions() const {
    return questions;
}