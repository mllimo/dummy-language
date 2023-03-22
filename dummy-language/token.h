#pragma once

enum TokenType
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
};

struct Token {
    TokenType type;
    std::string body;
};