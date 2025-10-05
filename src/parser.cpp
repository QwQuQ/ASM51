#include <string>
#include <vector>
#include <iostream>
#include <map>
#include "lexer.hpp"

#include "parser.hpp"

Parser::Parser(const std::vector<Token> &tokens)
    : tokens(tokens), position(0), currentAddress(0) {}

ParseResult Parser::parse()
{
    ParseResult result;

    while (position < tokens.size())
    {
        Token token = peek();
        std::cout << "\033[34mLexerOut:\033[0m" << "[" << token.type << "," << "\"" << token.value << "\"" << ",line:" << token.line << "]" << std::endl;
        if (isLabel(token))
        {
            std::string labelName = token.value.substr(0, token.value.size());
            std::cout << "\033[32mlabelname:\033[0m" << labelName << std::endl;
            result.symbolTable[labelName] = currentAddress;
            advance();
        }
        else if (isMnemonic(token))
        {
            // std::cout << "\033[33mMnemonic\033[0m" << std::endl;
            parseInstruction(result);
            for (const auto &i : result.instructions)
            {
                std::cout << "Instruction:";
                std::cout << i.mnemonic << std::endl;
                std::cout << "\t" << "operands:{" << std::endl;
                for (const auto &j : i.operands)
                {
                    std::cout << "\t" << "\t" << j << std::endl;
                }
                std::cout << "\t" << "}" << std::endl;
            }
            std::cout << "}" << std::endl;
        }
        else if (token.type == TokenType::PSEUDO)
        {
            std::cout << "\033[36mPSEUDO\033[0m" << std::endl;
            handlePseudo(result);
        }
        else
        {
            std::cout << "\033[31mOther\033[0m" << std::endl;
            advance();
        }
    }

    return result;
}

Token Parser::peek() const
{
    return position < tokens.size() ? tokens[position] : Token{TokenType::END_OF_FILE, "", -1};
}

Token Parser::advance()
{
    return position < tokens.size() ? tokens[position++] : Token{TokenType::END_OF_FILE, "", -1};
}

bool Parser::isLabel(const Token &token)
{
    return token.type == TokenType::LABEL && !token.value.empty();
}

bool Parser::isMnemonic(const Token &token)
{
    static const std::vector<std::string> mnemonics = {
        "MOV", "ADD", "SUBB", "INC", "DEC", "ANL", "ORL", "XRL",
        "CLR", "SETB", "CPL", "SJMP", "JMP", "JC", "JNC", "JZ", "JNZ",
        "DJNZ", "NOP", "ACALL", "LCALL", "RET", "RETI"};

    for (const auto &m : mnemonics)
    {
        if (token.value == m)
            return true;
    }
    return false;
}

void Parser::parseInstruction(ParseResult &result)
{
    Token mnemonicToken = advance();
    std::vector<std::string> operands;
    while (peek().type != TokenType::END_OF_LINE && peek().type != TokenType::END_OF_FILE && (!isMnemonic(peek())) && (peek().type == TokenType::NUMBER || peek().type == TokenType::ADDRESS || peek().type == TokenType::REGISTER || peek().type == TokenType::IMMEDIATE || peek().type == TokenType::COMMA || peek().type == TokenType::IDENTIFIER))
    {
        std::cout << "\033[34mpeek value:\033[0m" << "[" << peek().type << "," << "\"" << peek().value << "\"" << ",line:" << peek().line << "]" << std::endl;

        Token tok = advance();
        if (tok.type == TokenType::COMMA)
            continue;
        operands.push_back(tok.value);
    }

    Instruction instr{mnemonicToken.value, operands, mnemonicToken.line, currentAddress};
    result.instructions.push_back(instr);
    currentAddress += 1; // simplify: every instruction occupies single byte
}

void Parser::handlePseudo(ParseResult &result)
{
    Token pseudo = advance();
    if (pseudo.value == "ORG")
    {
        Token addr = advance();
        currentAddress = std::stoi(addr.value, nullptr, 0);
    }
    else if (pseudo.value == "DB")
    {
        while (peek().type != TokenType::END_OF_LINE && peek().type != TokenType::END_OF_FILE)
        {
            advance(); // ignoring DB data
            currentAddress += 1;
        }
    }
}