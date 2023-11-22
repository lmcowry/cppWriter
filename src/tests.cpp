#include "cppWriter.h"

#include <limits.h>

#include "gtest/gtest.h"

namespace
{
    static const std::string testTextFile = "./testText/testText.txt";

    TEST(countWordsInFile, Positive)
    {
        EXPECT_EQ(4, countWordsInFile(testTextFile));
    }

    TEST(getWordFromFileByWordNumber, Positive)
    {
        std::string expected = "hello";
        std::string observed = getWordFromFileByWordNumber(testTextFile, 0);
        EXPECT_EQ(expected, observed);

        expected = "there";
        observed = getWordFromFileByWordNumber(testTextFile, 1);
        EXPECT_EQ(expected, observed);

        expected = "good";
        observed = getWordFromFileByWordNumber(testTextFile, 2);
        EXPECT_EQ(expected, observed);

        expected = "bye";
        observed = getWordFromFileByWordNumber(testTextFile, 3);
        EXPECT_EQ(expected, observed);
    }

    TEST(stripPunctuation, With)
    {
        EXPECT_EQ("HELLO", stripPunctuation(",HELLO"));
        EXPECT_EQ("HELLO", stripPunctuation("HEL,LO"));
        EXPECT_EQ("HELLO", stripPunctuation("HELLO,"));
    }

    TEST(containsPunctuation, Without)
    {
        EXPECT_EQ("HELLO", stripPunctuation("HELLO"));
    }
}