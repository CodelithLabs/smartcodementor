#include <iostream>
#include <vector>
#include <string>
#include <map>

// --- Question Class ---
class Question {
private:
    std::string questionText;
    std::vector<std::string> options;
    int correctAnswerIndex; // 0-based index

public:
    Question(const std::string& qText, const std::vector<std::string>& opts, int correctIndex)
        : questionText(qText), options(opts), correctAnswerIndex(correctIndex) {}

    bool isCorrectAnswer(int userAnswer) const {
        // userAnswer is 1-based, convert to 0-based for comparison
        return (userAnswer - 1) == correctAnswerIndex;
    }

    int getCorrectAnswerIndex() const {
        return correctAnswerIndex;
    }

    const std::vector<std::string>& getOptions() const {
        return options;
    }

    std::string getQuestionText() const {
        return questionText;
    }
};

// --- Quiz Class ---
class Quiz {
private:
    std::string title;
    std::vector<Question> questions;
    std::string creatorUsername;

public:
    Quiz(const std::string& title, const std::string& creator)
        : title(title), creatorUsername(creator) {}

    void addQuestion(const Question& q) {
        questions.push_back(q);
    }

    const std::string& getTitle() const { return title; }
    const std::string& getCreatorUsername() const { return creatorUsername; }
    const std::vector<Question>& getQuestions() const { return questions; }

    void displayQuizSummary() const {
        std::cout << "Quiz Title: " << title << " (by " << creatorUsername << ")\n";
        std::cout << "Number of Questions: " << questions.size() << "\n";
    }
};

// --- QuizAttempt Class ---
class QuizAttempt {
private:
    std::string username;
    std::string quizTitle;
    int score;
    int totalQuestions;
    std::map<int, int> userAnswers;  // Question index -> user's chosen option (1-based)
    std::vector<bool> correctness;   // true = correct, false = incorrect

public:
    QuizAttempt(const std::string& user, const std::string& quizT, int totalQ)
        : username(user), quizTitle(quizT), score(0), totalQuestions(totalQ) {
        correctness.resize(totalQuestions, false);
    }

    void recordAnswer(int questionIndex, int userAnswer, bool isCorrect) {
        if (questionIndex < 0 || questionIndex >= totalQuestions)
            throw std::out_of_range("Invalid question index");

        userAnswers[questionIndex] = userAnswer;
        correctness[questionIndex] = isCorrect;
        if (isCorrect) score++;
    }

    int getScore() const { return score; }
    int getTotalQuestions() const { return totalQuestions; }
    const std::string& getUsername() const { return username; }
    const std::string& getQuizTitle() const { return quizTitle; }
    const std::map<int, int>& getUserAnswers() const { return userAnswers; }
    const std::vector<bool>& getCorrectness() const { return correctness; }

    bool isCorrectAnswer(int questionIndex, int userAnswer) const {
        if (questionIndex < 0 || questionIndex >= totalQuestions)
            throw std::out_of_range("Invalid question index");
        auto it = userAnswers.find(questionIndex);
        if (it != userAnswers.end()) {
            return it->second == userAnswer;
        }
        return false;
    }
};

// --- Example Usage ---
int main() {
    Quiz quiz("C++ Basics", "Prasanta");

    quiz.addQuestion(Question("What is the size of int in C++?", {"2 bytes", "4 bytes", "8 bytes"}, 1));
    quiz.addQuestion(Question("Which keyword defines a class?", {"struct", "class", "def"}, 1));

    quiz.displayQuizSummary();

    QuizAttempt attempt("Student01", quiz.getTitle(), quiz.getQuestions().size());

    const auto& questions = quiz.getQuestions();
    for (size_t i = 0; i < questions.size(); ++i) {
        std::cout << "\nQ" << i + 1 << ": " << questions[i].getQuestionText() << "\n";
        const auto& opts = questions[i].getOptions();
        for (size_t j = 0; j < opts.size(); ++j)
            std::cout << j + 1 << ". " << opts[j] << "\n";

        int ans;
        std::cout << "Your answer: ";
        std::cin >> ans;

        bool correct = questions[i].isCorrectAnswer(ans);
        attempt.recordAnswer(i, ans, correct);
    }

    std::cout << "\nFinal Score: " << attempt.getScore() << "/" << attempt.getTotalQuestions() << "\n";
    return 0;
}
