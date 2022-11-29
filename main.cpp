#include <fstream>
#include <iostream>
#include <vector>

#include <random>

#include <sqlite3.h>

static int callback(void* pObject, int columns, char** columnValues, char** columnNames)
{
    //https://stackoverflow.com/questions/29979782/c-correct-way-to-handle-sqlite3-prepare-and-sqlite3-step-errors
    // std::cout << "in the callback" << '\n';
    if (std::vector<std::string>* word = reinterpret_cast<std::vector<std::string>*>(pObject))
    {
        // std::cout << columnValues[1] << '\n';
        word->push_back(columnValues[1]);
        // std::cout << "push_back ed" << '\n';
    }

    return 0;
}

int main()
{
    //https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("/home/samuel_sewall/cppWriter/db/CppWriterDB.db", &DB); // whatever I put here, it'll create a new database


    if (exit)
    {
        std::cerr << "Error open DB " << sqlite3_errmsg(DB) << '\n';
        return (-1);
    }
    else
    {
        std::cout << "Opened Database Successfully!" << '\n';
    }

    std::vector<std::string> dbWords;
    std::string sql("SELECT * FROM WORDS_I_KNOW");
    int rc = sqlite3_exec(DB, sql.c_str(), &callback, static_cast<void*>(&dbWords), NULL);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Error SELECT" << '\n';
    }
    else
    {
        std::cout << "Operation OK!" << '\n';
    }

    sqlite3_close(DB); // probably should be in a try catch finally?
    

    std::string specificWord;

    //from https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomNum(1,dbWords.size()); // distribution in range [1, 2]

    specificWord = dbWords[randomNum(rng) - 1];
    //from https://www.learncpp.com/cpp-tutorial/basic-file-io/
    std::ofstream outf{ "/home/samuel_sewall/cppWriter/Sample.txt", std::ios::app };

    if (!outf)
    {
        std::cerr << "Uh oh, Sample.txt could not be opened for writing!\n";
        return 1;
    }

    outf << specificWord << '\n';
    return 0;
}