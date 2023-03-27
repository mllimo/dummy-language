#pragma once

#include <string>

enum class TokenType
{
    TokenType_UNDEFINED,
    ID,
    NUMBER,
    STRING,
    COMMENT,
    WHITE,
    TYPE,
    SEMICOLON,
    BINARY_OPERATOR,
    L_PARENTHESIS,
    R_PARENTHESIS
};

struct Token {
    TokenType type;
    std::string body;
};