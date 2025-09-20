#include "lexer.hpp"
#include <cctype>
#include <unordered_set>

std::ostream &operator<<(std::ostream &os, TokenType token)
{
    switch (token)
    {
    case TokenType::INSTRUCTION:
        os << "INSTRUCTION";
        break;
    case TokenType::REGISTER:
        os << "REGISTER";
        break;
    case TokenType::IMMEDIATE:
        os << "IMMEDIATE";
        break;
    case TokenType::ADDRESS:
        os << "ADDRESS";
        break;
    case TokenType::LABEL:
        os << "LABEL";
        break;
    case TokenType::PSEUDO:
        os << "PSEUDO";
        break;
    case TokenType::COMMA:
        os << "COMMA";
        break;
    case TokenType::COLON:
        os << "COLON";
        break;
    case TokenType::COMMENT:
        os << "COMMENT";
        break;
    case TokenType::IDENTIFIER:
        os << "IDENTIFIER";
        break;
    case TokenType::NUMBER:
        os << "NUMBER";
        break;
    case TokenType::END_OF_LINE:
        os << "END_OF_LINE";
        break;
    case TokenType::UNKNOWN:
    default:
        os << "Unknown";
        break;
    }
    return os;
}

Lexer::Lexer(const std::string &src) : source(src) {}

char Lexer::peek(size_t offset) const
{
    return (pos + offset < source.size()) ? source[pos + offset] : '\0';
}

char Lexer::advance()
{
    return (pos < source.size()) ? source[pos++] : '\0';
}

void Lexer::skipWhitespace()
{
    while (isspace(peek()))
    {
        if (peek() == '\n')
            line++;
        advance();
    }
}

std::vector<Token> Lexer::tokenize()
{
    std::vector<Token> tokens;
    while (pos < source.size())
    {
        skipWhitespace();
        if (peek() == ';')
        {
            tokens.push_back(lexComment());
        }
        else if (isalpha(peek()) || peek() == '_')
        {
            tokens.push_back(lexIdentifierOrKeyword());
        }
        else if (peek() == '#')
        {
            tokens.push_back(lexImmediate());
        }
        else if (isdigit(peek()))
        {
            tokens.push_back(lexNumber());
        }
        else if (peek() == ',')
        {
            advance();
            tokens.push_back({TokenType::COMMA, ",", line});
        }
        else if (peek() == ':')
        {
            advance();
            tokens.push_back({TokenType::COLON, ":", line});
        }
        else
        {
            tokens.push_back({TokenType::UNKNOWN, std::string(1, advance()), line});
        }
    }
    return tokens;
}

Token Lexer::lexComment()
{
    std::string value;
    while (peek() != '\n' && peek() != '\0')
    {
        value += advance();
    }
    return {TokenType::COMMENT, value, line};
}

Token Lexer::lexIdentifierOrKeyword()
{
    std::string word;
    while (isalnum(peek()) || peek() == '_')
    {
        word += advance();
    }

    std::string upper;
    for (char c : word)
        upper += toupper(c);

    if (isInstruction(upper))
        return {TokenType::INSTRUCTION, upper, line};
    if (isRegister(upper))
        return {TokenType::REGISTER, upper, line};
    if (isPseudo(upper))
        return {TokenType::PSEUDO, upper, line};

    if (peek() == ':')
    {
        advance();
        return {TokenType::LABEL, upper, line};
    }

    return {TokenType::IDENTIFIER, upper, line};
}

Token Lexer::lexNumber()
{
    std::string num;
    if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X'))
    {
        num += advance();
        num += advance(); // consume 0x
        while (isxdigit(peek()))
            num += advance();
    }
    else
    {
        while (isdigit(peek()))
            num += advance();
    }
    if (peek() == 'h' || peek() == 'H')
    {
        num = "0x" + num;
        advance(); // consume h or H
    }
    return {TokenType::NUMBER, num, line};
}

Token Lexer::lexImmediate()
{
    std::string imm;
    imm += advance(); // consume '#'
    if (peek() == '0' && (peek(1) == 'x' || peek(1) == 'X'))
    {
        imm += advance();
        imm += advance(); // consume 0x
        while (isxdigit(peek()))
            imm += advance();
    }
    else
    {
        while (isdigit(peek()))
            imm += advance();
    }
    if (peek() == 'h' || peek() == 'H')
    {
        imm = "0x" + imm.substr(1, imm.size() - 1);
        advance(); // consume h or H
    }
    return {TokenType::IMMEDIATE, imm, line};
}

bool Lexer::isInstruction(const std::string &word) const
{
    static const std::unordered_set<std::string> instructions = {
        "MOV", "ADD", "SUBB", "ANL", "ORL", "XRL", "INC", "DEC", "SJMP", "JMP", "NOP"};
    return instructions.count(word);
}

bool Lexer::isRegister(const std::string &word) const
{
    static const std::unordered_set<std::string> registers = {
        "A", "B", "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", "DPTR"};
    return registers.count(word);
}

bool Lexer::isPseudo(const std::string &word) const
{
    static const std::unordered_set<std::string> pseudos = {
        "ORG", "DB", "EQU", "END"};
    return pseudos.count(word);
}
