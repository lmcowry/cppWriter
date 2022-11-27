#include <fstream>
#include <iostream>

int main()
{
    std::cout << "hi there" << '\n';
    std::ofstream outf{ "/home/samuel_sewall/cppWriter/Sample.txt", std::ios::app };

    if (!outf)
    {
        std::cerr << "Uh oh, Sample.txt could not be opened for writing!\n";
        return 1;
    }

    outf << "This is line 5\n";
    outf << "This is line 6\n";
    std::cout << "bye" << '\n';
    return 0;
}