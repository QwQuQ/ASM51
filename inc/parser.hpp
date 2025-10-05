#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "lexer.hpp"

struct Instruction {
    std::string mnemonic;
    std::vector<std::string> operands;
    int line;
    int address;
};

struct ParseResult {
    std::vector<Instruction> instructions;
    std::map<std::string, int> symbolTable;
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    ParseResult parse();

private:
    std::vector<Token> tokens;
    size_t position;
    int currentAddress;

    Token peek() const;
    Token advance();
    bool isLabel(const Token& token);
    bool isMnemonic(const Token& token);
    void parseInstruction(ParseResult& result);
    void handlePseudo(ParseResult& result);
};