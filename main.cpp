#include <fstream>
#include <iostream>
#include <vector>

#include <random>

int main()
{
    // std::cout << "hi there" << '\n';
    std::vector<std::string> words {"hello", "goodbye"};
    std::string specificWord;

    //from https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomNum(1,words.size()); // distribution in range [1, 2]

    specificWord = words[randomNum(rng) - 1];
    //from https://www.learncpp.com/cpp-tutorial/basic-file-io/
    std::ofstream outf{ "/home/samuel_sewall/cppWriter/Sample.txt", std::ios::app };

    if (!outf)
    {
        std::cerr << "Uh oh, Sample.txt could not be opened for writing!\n";
        return 1;
    }

    outf << specificWord << '\n';
    // outf << "This is line 6\n";
    return 0;
}