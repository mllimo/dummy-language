#pragma once

#include "token.h"

#include <vector>
#include <list>

#include <string>
#include <regex>

Token Lexer(std::string& text)
{
    static const std::regex ID_x("([a-z]|[A-Z])(_|[0-9]|[a-z]|[A-Z])*");
    static const std::regex NUMBER_x("(\\+|-)?[0-9]+");
    static const std::regex STRING_x("\"[^\"]*\"");
    static const std::regex COMMENT("//[^\n]\n");
    static const std::regex WHITE_x("\\s+");
    static const std::regex TYPE_x("number|string");
    static const std::regex SEMICOLON_x(";");
    static const std::regex ASSING_OPERATOR_x("=");
    static const std::regex BINARY_OPERATOR_x("=|\\+|-|\\*|/");

    std::smatch matches;
    Token token;

    if (std::regex_search(text, matches, ID_x) && matches.position(0) == 0) token = { TokenType::ID,  matches[0] };
    if (std::regex_search(text, matches, NUMBER_x) && matches.position(0) == 0) token = { TokenType::NUMBER, matches[0] };
    if (std::regex_search(text, matches, STRING_x) && matches.position(0) == 0) token = { TokenType::STRING, matches[0] };
    if (std::regex_search(text, matches, COMMENT) && matches.position(0) == 0) token = { TokenType::COMMENT, matches[0] };
    if (std::regex_search(text, matches, WHITE_x) && matches.position(0) == 0) token = { TokenType::WHITE, matches[0] };
    if (std::regex_search(text, matches, TYPE_x) && matches.position(0) == 0) token = { TokenType::TYPE, matches[0] };
    if (std::regex_search(text, matches, SEMICOLON_x) && matches.position(0) == 0) token = { TokenType::SEMICOLON, matches[0] };
    if (std::regex_search(text, matches, BINARY_OPERATOR_x) && matches.position(0) == 0) token = { TokenType::BINARY_OPERATOR, matches[0] };


    text.erase(0, token.body.size());

    return token;
}

std::list<Token> Tokenize(std::string& text)
{
    std::list<Token> tokens;
    while (!text.empty()) {
        Token token = Lexer(text);
        if (token.type == TokenType::COMMENT || token.type == TokenType::WHITE) continue;
        tokens.emplace_back(std::move(token));
    }

    return tokens;
}