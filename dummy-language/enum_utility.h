#pragma once

#include "token.h"

#include <unordered_map>

template <typename ENUM>
std::string EnumToString(ENUM value) { return ""; }

template <>
std::string EnumToString<TokenType>(TokenType value)
{
    static const std::unordered_map<TokenType, std::string> map = {
        {TokenType::TokenType_UNDEFINED, "UNDEFINED"},
        {TokenType::ID, "ID"},
        {TokenType::NUMBER, "NUMBER"},
        {TokenType::STRING, "STRING"},
        {TokenType::COMMENT, "COMMENT"},
        {TokenType::WHITE, "WHITE"},
        {TokenType::TYPE, "TYPE"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::BINARY_OPERATOR, "BINARY_OPERATOR"},
        {TokenType::L_PARENTHESIS, "L_PARENTHESIS"},
        {TokenType::R_PARENTHESIS, "R_PARENTHESIS"}
    };

    auto it = map.find(value);
    if (it == map.end()) throw std::runtime_error("EnumToString<TokenType> fails");
    return it->second;
}
