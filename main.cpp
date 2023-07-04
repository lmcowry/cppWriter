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

sqlite3* gDB;

std::vector<std::string> gDBWords;
std::vector<DatabaseWord> gCurrentWords;
DatabaseWord gCurrentTrueWord;
std::string gGuessWord;

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

static int callbackWithClass(void* pObject, int columns, char** columnValues, char** columnNames)
{
    //https://stackoverflow.com/questions/29979782/c-correct-way-to-handle-sqlite3-prepare-and-sqlite3-step-errors
    // std::cout << "in the callback" << '\n';
    if (std::vector<DatabaseWord>* databaseWord = reinterpret_cast<std::vector<DatabaseWord>*>(pObject))
    {
        DatabaseWord x;
        x.id = atoi(columnValues[0]);
        x.word = columnValues[1];
        databaseWord->push_back(x);
    }

    return 0;
}

void connectToDatabase()
{
    int exit = 0;
    exit = sqlite3_open("/home/samuel_sewall/cppWriter/db/CppWriterDB.db", &gDB); // whatever I put here, it'll create a new database

    if (exit)
    {
        std::cerr << "Error open DB " << sqlite3_errmsg(gDB) << '\n';
        std::exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Opened Database Successfully!" << '\n';
    }
}

void parseResponseCode(int rc, std::string successMessage, std::string errorMessage)
{
    if (rc != SQLITE_OK)
    {
        std::cerr << errorMessage << '\n';
    }
    else
    {
        std::cout << successMessage << '\n';
    }
}

void getVocab()
{
    std::string sql("SELECT * FROM WORDS_I_KNOW");
    int rc = sqlite3_exec(gDB, sql.c_str(), &callback, static_cast<void*>(&gDBWords), NULL);
    parseResponseCode(rc, "Operation OK!", "Error " + sql);
}

void getCurrentWordAndWordNumber()
{
    
    std::string sql("SELECT * FROM CURRENT_TRUE_WORD WHERE ID = (SELECT MAX(ID) FROM CURRENT_TRUE_WORD)");
    int rc = sqlite3_exec(gDB, sql.c_str(), &callbackWithClass, static_cast<void*>(&gCurrentWords), NULL);
    parseResponseCode(rc, "Operation OK!", "Error " + sql);
    gCurrentTrueWord = gCurrentWords[0];
}

int getRandomNumber()
{
    //from https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
    //could replace below with sqlite3's random()
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> randomNum(1,gDBWords.size()); // distribution in range [1, 2]
    return randomNum(rng) - 1;
}

void deleteTruePartialPlusGuessThenMakeNew()
{
    std::cout << "trying rm and cp" << '\n';
    system("rm /home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt");
    system("cp /home/samuel_sewall/cppWriter/TruePartial.txt /home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt");
    std::cout << "did the rm and cp" << '\n';
}

void writeGuessWordToFile()
{
    //from https://www.learncpp.com/cpp-tutorial/basic-file-io/
    std::ofstream outf{ "/home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt", std::ios::app };

    if (!outf)
    {
        std::cerr << "Uh oh, text file could not be opened for writing!\n";
        std::exit(EXIT_FAILURE);
    }

    outf << ' ' << gGuessWord;
}

void checkTrueVsGuess()
{
    std::cout << "gGuessWord = " + gGuessWord + " and gCurrentTrueWord.word = " + gCurrentTrueWord.word << '\n';
    std::cout << (gGuessWord == gCurrentTrueWord.word) << '\n';
    if (gGuessWord == gCurrentTrueWord.word)
    {
        std::cout << "they're equal" << '\n';

        //open TruePartial
        std::ofstream outf2{ "/home/samuel_sewall/cppWriter/TruePartial.txt", std::ios::app };
        if (!outf2)
        {
            std::cerr << "Uh oh, text file could not be opened for writing!\n";
            std::exit(EXIT_FAILURE);
        }
        //add guessWord to TruePartial
        outf2 << ' ' << gGuessWord;

        //read TrueFull to get new current word
        std::ifstream inf{ "/home/samuel_sewall/cppWriter/TrueFull.txt" };
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
            std::cout << counter << '\n';
        }
        std::cout << '\n' << "read TrueFull" << '\n';
        std::cout << strInput << '\n';
    
        // split strInput into vector of strings, split by spaces
        //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
        std::string s;
        std::stringstream ss(strInput);
        std::vector<std::string> v;
        int wordCounter = 1;

        while (getline(ss, s, ' ') || wordCounter != gCurrentTrueWord.id)
        {
            v.push_back(s);
            wordCounter++;
        }

        std::cout << "new word is: " << v[wordCounter] << '\n';

        

        //insert that as new CURRENT_TRUE_WORD
    }

}


int main()
{
    connectToDatabase();
    getVocab();
    getCurrentWordAndWordNumber();
    gGuessWord = gDBWords[getRandomNumber()];
    deleteTruePartialPlusGuessThenMakeNew();
    writeGuessWordToFile();
    checkTrueVsGuess();
    sqlite3_close(gDB);
    return 0;
}