// #include <fstream>
#include <iostream>
// #include <vector>
// #include <sstream>

// #include <random>

// #include <sqlite3.h>

void changeValue(int& input)
{
    input = 5;
}

void middleMan(int& input)
{
    changeValue(input);
}

void changeValuePointer(int** input)
{
    **input = 6;
}

void middleManPointer(int& input)
{
    std::cout << input << '\n';
    // std::cout << *input << '\n'; // doesn't work since input not a pointer
    std::cout << &input << '\n';
    
    changeValuePointer((int**)input);
}

int main()
{
    int x = 0;
    middleMan(x);
    std::cout << x << '\n';
    //-----end-----
    int* xp = &x;
    middleManPointer(*xp);
    std::cout << x << '\n';
    return 0;
}