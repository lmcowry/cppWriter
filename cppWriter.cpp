#include "cppWriter.h"

sqlite3* gDB;

std::vector<std::string> gDBWords;
std::vector<DatabaseWord> gCurrentWords;
DatabaseWord gCurrentTrueWord;
std::string gGuessWord;

std::string truePartialFile = "/home/samuel_sewall/cppWriter/TruePartial.txt";
std::string truePartialPlusGuessFile = "/home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt";
std::string trueFullFile = "/home/samuel_sewall/cppWriter/TrueFull.txt";

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

void parseResponseCode(int rc, std::string successMessage, std::string errorMessage, char* longErrorMessage)
{
    if (rc != SQLITE_OK)
    {
        std::cerr << errorMessage << '\n';
        std::cerr << longErrorMessage << '\n';
    }
    else
    {
        std::cout << successMessage << '\n';
    }
}

void getVocab()
{
    std::string sql("SELECT * FROM WORDS_I_KNOW");
    char* error = nullptr;
    int rc = sqlite3_exec(gDB, sql.c_str(), &callback, static_cast<void*>(&gDBWords), &error);
    parseResponseCode(rc, "Operation OK!", "Error " + sql, error);
}

void getCurrentWordAndWordNumber()
{
    
    std::string sql("SELECT ROWID, WORD FROM CURRENT_TRUE_WORD WHERE ROWID = (SELECT MAX(ROWID) FROM CURRENT_TRUE_WORD)");
    char* error = nullptr;
    int rc = sqlite3_exec(gDB, sql.c_str(), &callbackWithClass, static_cast<void*>(&gCurrentWords), &error);
    parseResponseCode(rc, "Operation OK!", "Error " + sql, error);
    gCurrentTrueWord = gCurrentWords[0];
    std::cout << "gCurrentTrueWord is " << gCurrentTrueWord.word << '\n';
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
    system("rm /home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt");
    system("cp /home/samuel_sewall/cppWriter/TruePartial.txt /home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt");
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
    // if they're the same, then need to get new current true word
    std::cout << "gGuessWord = " + gGuessWord + " and gCurrentTrueWord.word = " + gCurrentTrueWord.word << '\n';
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
        }
    
        // split strInput into vector of strings, split by spaces
        //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
        std::string s;
        std::stringstream ss(strInput);
        std::vector<std::string> v;
        int wordCounter = 1;

        // std::cout << "word id is " << gCurrentTrueWord.id << '\n';

        while (getline(ss, s, ' ')) // could add "|| wordCounter != gCurrentTrueWord.id" 
        {
            v.push_back(s);
            wordCounter++;
        }

        std::cout << "new word is: " << v[wordCounter] << '\n';

        

        //insert that as new CURRENT_TRUE_WORD
    }

}

int getWordCount(std::string filePath)
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
    std::cout << "read " + filePath << '\n';

    // split strInput into vector of strings, split by spaces
    //https://www.geeksforgeeks.org/split-string-by-space-into-vector-in-cpp-stl/#
    std::string s;
    std::stringstream ss(strInput);
    std::vector<std::string> v;
    int wordCounter = 1;

    // std::cout << "word id is " << gCurrentTrueWord.id << '\n';

    while (getline(ss, s, ' ')) // could add "|| wordCounter != gCurrentTrueWord.id" 
    {
        wordCounter++;
    }

    return wordCounter - 1; //why do I have to -1 this?
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

int wordCounter(std::string filename)
{
    std::string command{"wc -w "};
    std::string piped{" | grep '[[:digit:]]' -o"};
    command += filename + piped;


    // from here: https://stackoverflow.com/questions/51810647/how-to-simply-use-console-commands-in-a-c-program
    char buffer[10];
    FILE *f = popen(command.c_str(), "r");
    int wcInt = 0;
    while ( fgets(buffer, 10, f) != nullptr )
    {
        wcInt = atoi(buffer);
    }

    pclose( f );

    return wcInt;
}

bool checkIfBeginning()
{
    return (wordCounter("/home/samuel_sewall/cppWriter/TruePartial.txt") == 0) ? true : false;
}

bool checkIfEnd()
{
    int truePartialCount = wordCounter(truePartialFile);
    int trueFullCount = wordCounter(trueFullFile);
    return truePartialCount == trueFullCount;
}

void makeCurrentTrueWord(int wordCount)
{
    //read next word in TrueFull.txt
    std::string next_current_true_word = getWord("/home/samuel_sewall/cppWriter/TrueFull.txt", wordCount);
    std::cout << "next word read from TrueFull.txt = " + next_current_true_word << '\n';

    //update current_true_word
    std::string sql("INSERT INTO CURRENT_TRUE_WORD(WORD) VALUES('" + next_current_true_word + "');");
    char* error = nullptr;
    int rc = sqlite3_exec(gDB, sql.c_str(), &callback, static_cast<void*>(&gDBWords), &error); //including callback, but shouldn't be called, since this is an insert
    parseResponseCode(rc, "Operation OK!", "Error " + sql, error);
}

#ifndef GTEST
int main()
{
    connectToDatabase();
    if (checkIfEnd())
    {
        std::cout << "last word complete, no need to run further" << '\n';
    }
    else
    {
        if (checkIfBeginning())
        {
            makeCurrentTrueWord(0);
            std::cout << "first word added" << '\n';
        }
        getCurrentWordAndWordNumber();
        // if (gFinalWord)
        // {
        //     std::cout << "last word complete, no need to run further" << '\n';
        //     return 0;
        // }
        getVocab();
        gGuessWord = gDBWords[getRandomNumber()];
        deleteTruePartialPlusGuessThenMakeNew();
        writeGuessWordToFile();
        std::cout << "gGuessWord is " << gGuessWord << " and currentTrueWord is " << gCurrentTrueWord.word << '\n';
        if (gGuessWord == gCurrentTrueWord.word)
        {
            //update TruePartial.txt
            system("cp /home/samuel_sewall/cppWriter/TruePartialPlusGuess.txt /home/samuel_sewall/cppWriter/TruePartial.txt");

            //read TruePartial.txt to get word count
            //could instead execute "wc TruePartial.txt -w", but would have to somehow get that output
            int wordCount = getWordCount("/home/samuel_sewall/cppWriter/TruePartial.txt");
            std::cout << "TruePartial word count = " << wordCount << '\n';

            makeCurrentTrueWord(wordCount);


        }
    }
    sqlite3_close(gDB);
    return 0;
    
}
#endif