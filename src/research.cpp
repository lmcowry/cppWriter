// #include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

// #include <random>

// #include <sqlite3.h>

std::string vectorPractice()
{
        //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
    std::string strInput = "hello there my name is christian   ";
    std::string s;
    std::stringstream ss(strInput);
    std::vector<std::string> v;
    int wordCounter = 1;

    while (getline(ss, s, ' ')) //so the issue seems to be that getline fucks up when it gets to multiple delimiters.
    //I'm guessing in main.cpp, it gets real confused with end of file stuff
    {
        v.push_back(s);
        wordCounter++;
    }

    for (uint i = 0; i < v.size(); i++)
    {
        std::cout << v[i] << '\n';
    }

    std::cout << wordCounter << '\n';
    std::cout << "new word is: " << v[wordCounter - 2] << '\n'; // why do I need to -2 it for the last word?
    return v[wordCounter - 2];
}

std::string splitThatString(int whatWord)
{
    //https://favtutor.com/blogs/split-string-cpp
    std::string s = "I love to read articles on Favtutor.";
    
    std::stringstream ss(s);  
    std::string word;
    std::vector<std::string> v;
    while (ss >> word)
    {
        std::cout << word << '\n';
        v.push_back(word);
    }

    return v[whatWord];
}

void mysteries()
{
    char x[] = "hello there";
    std::cout << x + 8 << '\n'; //why does this produce "ere"
    std::cout << x << 8 << '\n';//whereas this behaves as you'd expect
}

void wcTest()
{
    std::string filename{"/home/samuel_sewall/cppWriter/TruePartial.txt"};
    std::string command{"wc -w "};
    std::string piped{" | grep '[[:digit:]]' -o"};
    command += filename + piped;

    char buffer[10];
    FILE *f = popen(command.c_str(), "r");
    int wcInt = 0;
    while ( fgets(buffer, 10, f) != nullptr )
    {
        std::cout << buffer << std::endl;
        wcInt = atoi(buffer);
    }

    pclose( f );
    std::cout << "and the word count is " << wcInt << '\n';
}

std::string getWord(std::string filePath, int wordNumber)
{

    //read TrueFull to get new current word
    std::ifstream inf{ filePath };
    if (!inf)
    {
        // Print an error and exit
        std::cerr << "Uh oh, text file could not be opened for reading!\n";
        std::exit(EXIT_FAILURE);
    }
    std::string strInput;
    int counter = 0;
    while (inf)
    {
        // read stuff from the file into a string
        std::getline(inf, strInput);
        inf >> strInput;
        counter++;
    }

    // split strInput into vector of strings, split by spaces
    //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
    std::string s;
    std::stringstream ss(strInput);
    std::vector<std::string> v;
    int wordCounter = 0;

    // std::cout << "word id is " << gCurrentTrueWord.id << '\n';

    while ((ss >> s) && wordCounter <= wordNumber) 
    {
        v.push_back(s);
        // if (wordCounter == wordNumber)
        // {
        //     gFinalWord = true;
        // }
        wordCounter++;
    }

    wordCounter--;

    std::cout << "word is " << v[wordCounter] << '\n';

    return v[wordCounter];
}

std::string stripPunctuation(std::string input)
{
    std::string newString;
    for (uint i = 0; i < input.length(); i++)
    {
        //some from https://stackoverflow.com/questions/34379606/how-could-i-count-some-punctuation-marks-which-the-function-ispunct-doesnt-have
        static const std::string punctuations(".,-;:'\"()?");
        if (punctuations.find(input[i]) != std::string::npos)
        {

        }
        else
        {
            newString += input[i];
        }
    }
    return newString;
}

//I just can't remember off the top of my head how to
//pass by reference, whether the argument is a dereferenced pointer or a regular value
//this is my notes for it
void practice(int& x)
{
    x = 5;
    std::cout << "within practice, x is " << x << '\n';
}

void practiceCaller()
{
    int myNumber = 7;
    std::cout << myNumber << '\n';
    practice(myNumber);
    std::cout << myNumber << '\n';

    int z = 6;
    int* pMyNumber = &z;
    std::cout << *pMyNumber << '\n';
    practice(*pMyNumber);
    std::cout << *pMyNumber << '\n';
}

void upperCase(std::string x)
{
    std::cout << x << '\n';
    std::transform(x.begin(), x.end(), x.begin(), ::toupper);
    std::cout << x << '\n';
}

int main()
{
    upperCase("hel,lo");
}