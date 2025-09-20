#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.hpp"

int main(int argc, char *argv[])
{
    std::string code = argv[1];
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    for (Token i : tokens)
    {
        std::cout << "[" << i.type << "," << "\"" << i.value << "\"" << ",line:" << i.line << "]" << std::endl;
    }
    return 0;
}