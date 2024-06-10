#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
    // Expect two strings not to be equal.
    EXPECT_STRNE("hello", "world");
    // Expect equality.
    EXPECT_EQ(7 * 6, 42);
}

// Do some more??
TEST(HelloTest, MoreAssertions) {
    // Expect two strings to be equal.
    EXPECT_STREQ("hello", "hello");
}

