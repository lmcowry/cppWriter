#include "cppWriter.h"

#include <limits.h>

#include "gtest/gtest.h"

namespace
{
    TEST(MyOwnTest, Positive)
    {
        EXPECT_EQ(2, wordCounter("/home/samuel_sewall/cppWriter/project/testText/testText.txt"));
    }
}