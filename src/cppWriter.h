#ifndef CPP_WRITER_H
#define CPP_WRITER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>

#include <random>

#include <sqlite3.h>

class DatabaseWord
{
    public:
        int id;
        std::string word;
};

static sqlite3* gDB;

static std::vector<std::string> gDBWords;
static std::vector<DatabaseWord> gCurrentWords;
static DatabaseWord gCurrentTrueWord;
static std::string gGuessWord;

static const std::string truePartialFile = "./trueText/TruePartial.txt";
static const std::string truePartialPlusGuessFile = "./trueText/TruePartialPlusGuess.txt";
static const std::string trueFullFile = "./trueText/TrueFull.txt";


int countWordsInFile(std::string filename);
std::string getWordFromFileByWordNumber(std::string filePath, int wordNumber);


#endif // CPP_WRITER_H