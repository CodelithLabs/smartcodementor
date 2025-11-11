#include <gtest/gtest.h>
#include "QuizLogic.h" // Include the logic you want to test

// Define a test case
TEST(MathTest, Addition) {
    // Check if add(2, 3) equals 5
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(-1, 1), 0);
}