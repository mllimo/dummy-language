#pragma once

#include <new>
#include <exception>
#include <list>

#include "token.h"
#include "enum_utility.h"

void CheckToken(const Token& token, TokenType expected_type)
{
    if (token.type != expected_type) throw std::runtime_error("Expected " + EnumToString(expected_type) + " but got " + EnumToString(token.type));
}

bool CheckToken(std::nothrow_t, const Token& token, TokenType expected_type)
{
    if (token.type != expected_type) return false;
    return true;
}

bool CheckNext(std::list<Token>::iterator it, TokenType expected_type)
{
    ++it;
    return it->type == expected_type;
}