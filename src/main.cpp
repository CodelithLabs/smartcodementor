#include <iostream>
#include <string>
#include <vector>
#include <stdexcept> // For std::out_of_range
#include <utility>   // For std::move

// We assume Question, Quiz, and QuizAttempt are all defined in this file,
// based on the compilation errors.
//
// NOTE: For a more professional structure, these classes should be in
// their own .h and .cpp files (e.g., Quiz.h, Quiz.cpp) and be part of
// the 'quiz_lib' library, not defined in main.cpp.

// --- Question Class (Inferred) ---
// We need a minimal definition for the code to compile.
class Question {
private:
    std::string text;
    std::vector<std::string> options;
    int correctAnswerIndex; // Assuming answer index is an int
public:
    Question(std::string txt, std::vector<std::string> opts, int correct)
        : text(std::move(txt)), options(std::move(opts)), correctAnswerIndex(correct) {}

    const std::string& getText() const { return text; }
    const std::vector<std::string>& getOptions() const { return options; }
    
    // Check if a given answer index is correct
    bool isCorrect(int answerIndex) const {
        return answerIndex == correctAnswerIndex;
    }
};

// --- Quiz Class ---
class Quiz {
private:
    std::string title; // This was line 39
    std::string creator;
    std::vector<Question> questions;

public:
    // This was line 44
    // FIX: Renamed parameter 'inTitle' to 'newTitle' to avoid shadowing member 'title'.
    Quiz(const std::string& newTitle, const std::string& newCreator)
        : title(newTitle), creator(newCreator) {
        // Constructor body
    }

    void addQuestion(const Question& question) {
        questions.push_back(question);
    }

    const std::string& getTitle() const { return title; }
    const std::vector<Question>& getQuestions() const { return questions; }
};

// --- QuizAttempt Class ---
class QuizAttempt {
private:
    std::string studentId;
    std::string quizTitle;
    int score;
    std::vector<bool> correctness; // Vector to track correct answers

public:
    // This was line 74
    // FIX: Changed 'totalQuestions' from 'int' to 'size_t' to match std::vector::resize.
    // 'size_t' is the correct type for sizes and counts in C++.
    QuizAttempt(const std::string& id, const std::string& qTitle, size_t totalQuestions)
        : studentId(id), quizTitle(qTitle), score(0) {
        // This line is now safe, no sign conversion.
        correctness.resize(totalQuestions, false);
    }

    // This was line 82
    // FIX: Changed 'questionIndex' from 'int' to 'size_t' to match std::vector index.
    void recordAnswer(size_t questionIndex, int answer, bool isCorrect) {
        if (isCorrect) {
            score++;
        }

        // Add bounds check for safety
        if (questionIndex < correctness.size()) {
            // This line is now safe, no sign conversion.
            correctness[questionIndex] = isCorrect;
        } else {
            // Handle error, e.g., log it or throw
            std::cerr << "Error: questionIndex " << questionIndex << " out of bounds." << std::endl;
        }
    }

    int getScore() const { return score; }
    size_t getTotalQuestions() const { return correctness.size(); }
};

// --- main() function ---
int main() {
    std::cout << "Starting SmartCodeMentor Quiz..." << std::endl;

    // 1. Create a quiz
    Quiz quiz("C++ Basics", "Admin");
    quiz.addQuestion(Question("What is a vector?", {"Dynamic array", "Static array", "A number"}, 0));
    quiz.addQuestion(Question("What does 'std' stand for?", {"Standard", "Student", "Stead"}, 0));
    quiz.addQuestion(Question("What is 'size_t'?", {"An integer type for sizes", "A float", "A char"}, 0));

    // 2. Create a quiz attempt
    // This was line 113
    // FIX: quiz.getQuestions().size() returns size_t, which now correctly
    // matches the QuizAttempt constructor. No data loss or conversion error.
    QuizAttempt attempt("Student01", quiz.getTitle(), quiz.getQuestions().size());

    const auto& questions = quiz.getQuestions();
    
    // 3. Simulate answering the quiz
    // This was line ~127
    // Loop uses size_t, which is the correct type for vector indexing.
    for (size_t i = 0; i < questions.size(); ++i) {
        // Simulate a user answering. Let's just say they get it right.
        int dummyAnswer = 0; // User picks first option
        bool correct = questions[i].isCorrect(dummyAnswer);
        
        // FIX: 'i' is size_t, which now correctly matches recordAnswer's
        // new signature (which also expects size_t).
        attempt.recordAnswer(i, dummyAnswer, correct);
    }

    // 4. Print the score
    std::cout << "Quiz complete." << std::endl;
    std::cout << "Score: " << attempt.getScore() << " / " << attempt.getTotalQuestions() << std::endl;

    return 0;
}