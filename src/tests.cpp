#include "cppWriter.h"

#include <limits.h>

#include "gtest/gtest.h"

namespace
{
    static const std::string testTextFile = "./testText/testText.txt";

    TEST(countWordsInFile, Positive)
    {
        EXPECT_EQ(2, countWordsInFile(testTextFile));
    }

    TEST(getWordFromFileByWordNumber, Positive)
    {
        std::string expected = "hello";
        std::string observed = getWordFromFileByWordNumber(testTextFile, 0);
        EXPECT_EQ(expected, observed);

        expected = "there";
        observed = getWordFromFileByWordNumber(testTextFile, 1);
        EXPECT_EQ(expected, observed);

        expected = "there";
        observed = getWordFromFileByWordNumber(testTextFile, 2);
        EXPECT_EQ(expected, observed);

        expected = "there";
        observed = getWordFromFileByWordNumber(testTextFile, 3);
        EXPECT_EQ(expected, observed);
    }
}