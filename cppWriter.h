#ifndef GOOGLETEST_MAIN_H_
#define GOOGLETEST_MAIN_H_

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



int wordCounter(std::string filename);


#endif // GOOGLETEST_MAIN_H_