// #include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

// #include <random>

// #include <sqlite3.h>

void vectorPractice()
{
        //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
    std::string strInput = "hello there my name is christian ";
    std::string s;
    std::stringstream ss(strInput);
    std::vector<std::string> v;
    int wordCounter = 1;

    while (getline(ss, s, ' '))
    {
        v.push_back(s);
        wordCounter++;
    }

    for (int i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << '\n';
    }

    std::cout << wordCounter << '\n';
    std::cout << "new word is: " << v[wordCounter - 2] << '\n'; // why do I need to -2 it for the last word?
}

void mysteries()
{
    char x[] = "hello there";
    std::cout << x + 8 << '\n'; //why does this produce "ere"
    std::cout << x << 8 << '\n';//whereas this behaves as you'd expect
}

int main()
{
    vectorPractice();
}