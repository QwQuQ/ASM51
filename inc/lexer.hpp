#pragma once
#include <string>
#include <vector>
#include <iostream>

enum class TokenType
{
    INSTRUCTION,
    REGISTER,
    IMMEDIATE,
    ADDRESS,
    LABEL,
    PSEUDO,
    COMMA,
    COLON,
    COMMENT,
    IDENTIFIER,
    NUMBER,
    END_OF_LINE,
    UNKNOWN
};

std::ostream &operator<<(std::ostream &os, TokenType type);

struct Token
{
    TokenType type;
    std::string value;
    int line;
};

class Lexer
{
public:
    Lexer(const std::string &source);
    std::vector<Token> tokenize();

private:
    std::string source;
    size_t pos = 0;
    int line = 1;

    char peek(size_t offset = 0) const;
    char advance();
    void skipWhitespace();
    Token lexToken();
    Token lexIdentifierOrKeyword();
    Token lexNumber();
    Token lexImmediate();
    Token lexComment();

    bool isInstruction(const std::string &word) const;
    bool isRegister(const std::string &word) const;
    bool isPseudo(const std::string &word) const;
};