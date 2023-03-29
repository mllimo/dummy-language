#include "checkers.h"

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

bool CheckOnOf(const Token& token, const std::initializer_list<TokenType>& types)
{
    return CheckOnOf(token.type, types);
}


bool CheckOnOf(const TokenType& token, const std::initializer_list<TokenType>& types)
{
    for (auto& type : types) {
        if (token == type) return true;
    }

    return false;
}