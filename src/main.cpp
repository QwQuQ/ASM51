#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.hpp"
#include "lexer.hpp"
#include "parser.hpp"

int main(int argc, char *argv[])
{
    std::string code = readAsmFile("test.asm");
    Lexer lexer(code);
    std::vector<Token> tokens = lexer.tokenize();

    // for (const auto &i : tokens)
    // {
    //     std::cout << "[" << i.type << "," << "\"" << i.value << "\"" << ",line:" << i.line << "]" << std::endl;
    // }

    Parser parser(tokens);
    ParseResult parse_results = parser.parse();
    return 0;
}
