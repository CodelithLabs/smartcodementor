#pragma once

#include <string>
#include <vector>
#include "Question.h" // We include Question.h because Quiz "has a" vector of Questions

class Quiz {
private:
    std::string title;
    std::string creator;
    std::vector<Question> questions;

public:
    Quiz(const std::string& newTitle, const std::string& newCreator);

    void addQuestion(const Question& question);
    const std::string& getTitle() const;
    const std::vector<Question>& getQuestions() const;
};