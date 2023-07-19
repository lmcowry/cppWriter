#include "cppWriter.h"

static int callback(void* pObject, int, char** columnValues, char**)
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

static int callbackWithClass(void* pObject, int, char** columnValues, char**)
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
    exit = sqlite3_open("./db/CppWriterDB.db", &gDB); // whatever I put here, it'll create a new database

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

void getKnownWordsFromDatabase()
{
    std::string sql("SELECT * FROM WORDS_I_KNOW");
    char* error = nullptr;
    int rc = sqlite3_exec(gDB, sql.c_str(), &callback, static_cast<void*>(&gDBWords), &error);
    parseResponseCode(rc, "Operation OK!", "Error " + sql, error);
}

void getCurrentWordAndWordNumberFromDatabase()
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
    system("rm ./trueText/TruePartialPlusGuess.txt");
    system("cp ./trueText/TruePartial.txt ./trueText/TruePartialPlusGuess.txt");
}

void writeGuessWordToFile()
{
    //from https://www.learncpp.com/cpp-tutorial/basic-file-io/
    std::ofstream outf{ "./trueText/TruePartialPlusGuess.txt", std::ios::app };

    if (!outf)
    {
        std::cerr << "Uh oh, text file could not be opened for writing!\n";
        std::exit(EXIT_FAILURE);
    }

    outf << ' ' << gGuessWord;
}

// zero indexed
// if given wordNumber greater than number of words in file, will return last word
std::string getWordFromFileByWordNumber(std::string filePath, int wordNumber)
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

    return v[wordCounter - 1];
}

//one indexed
int countWordsInFile(std::string filename)
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
    return (countWordsInFile("./trueText/TruePartialPlusGuess.txt") == 0) ? true : false;
}

bool checkIfEnd()
{
    int truePartialCount = countWordsInFile(truePartialFile);
    int trueFullCount = countWordsInFile(trueFullFile);
    return truePartialCount == trueFullCount;
}

void makeCurrentTrueWordInDatabase(int wordCount)
{
    //read next word in TrueFull.txt
    std::string next_current_true_word = getWordFromFileByWordNumber("./trueText/TrueFull.txt", wordCount);
    std::cout << "next word read from TrueFull.txt = " + next_current_true_word << '\n';

    //update current_true_word
    std::string sql("INSERT INTO CURRENT_TRUE_WORD(WORD) VALUES('" + next_current_true_word + "');");
    char* error = nullptr;
    int rc = sqlite3_exec(gDB, sql.c_str(), &callback, static_cast<void*>(&gDBWords), &error); //including callback, but shouldn't be called, since this is an insert
    parseResponseCode(rc, "Operation OK!", "Error " + sql, error);
}

bool containsPunctuation(std::string input)
{
    for (uint i = 0; i < input.length(); i++)
    {
        //some from https://stackoverflow.com/questions/34379606/how-could-i-count-some-punctuation-marks-which-the-function-ispunct-doesnt-have
        static const std::string punctuations(".,-;:'\"()?");
        if (punctuations.find(input[i]) != std::string::npos)
        {
            return true;
        }
    }
    return false;

}

bool checkGuessWord()
{
    return gGuessWord == gCurrentTrueWord.word;
}

#ifndef GTEST
int main(int argc, char *argv[])
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
            makeCurrentTrueWordInDatabase(0);
            std::cout << "first word added" << '\n';
        }
        if (argc == 2)
        {
            gCurrentTrueWord.word = argv[1];
        }
        else
        {
            getCurrentWordAndWordNumberFromDatabase();
        }

        getKnownWordsFromDatabase();
        gGuessWord = gDBWords[getRandomNumber()];
        deleteTruePartialPlusGuessThenMakeNew();
        writeGuessWordToFile();
        std::cout << "gGuessWord is " << gGuessWord << " and currentTrueWord is " << gCurrentTrueWord.word << '\n';

        if (checkGuessWord())
        {
            //update TruePartial.txt
            system("cp ./trueText/TruePartialPlusGuess.txt ./trueText/TruePartial.txt");

            //read TruePartial.txt to get word count
            //could instead execute "wc TruePartial.txt -w", but would have to somehow get that output
            int wordCount = countWordsInFile(truePartialFile);
            std::cout << "TruePartial word count = " << wordCount << '\n';

            makeCurrentTrueWordInDatabase(wordCount);


        }
    }
    sqlite3_close(gDB);
    return 0;
    
}
#endif