#include "Lexer/lexer.hpp"
#include <iostream>
#include <format>

int main(const int argc , char *argv[])
{
    if (argc == 1)
    {
        std::cout << "No input file provided.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    if (argc == 2)
    {
        run_file(argv[1]);
        return 0;
    }
    if (argc > 2)
    {
        std::cout << "Too many arguments.\n";
        std::cout << std::format("Usage: {} <file.at>\n", argv[0]);
        std::cout << "Program terminated.\n";
        return 1;
    }
    return 0;
}