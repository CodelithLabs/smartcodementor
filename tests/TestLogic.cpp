#include <gtest/gtest.h>
#include "Question.h" // Include the logic we want to test

// Define a test case for the Question class
TEST(QuestionTest, CorrectAnswer) {
    Question q("What is 2+2?", {"3", "4", "5"}, 1);
    
    // Check if the correct answer (index 1) is identified
    EXPECT_TRUE(q.isCorrect(1));
    
    // Check if an incorrect answer (index 0) is identified
    EXPECT_FALSE(q.isCorrect(0));
}

// You can add more tests for Quiz and QuizAttempt here!