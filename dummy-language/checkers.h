#pragma once

#include <new>
#include <exception>
#include <list>

#include "token.h"
#include "enum_utility.h"

void CheckToken(const Token& token, TokenType expected_type);
bool CheckToken(std::nothrow_t, const Token& token, TokenType expected_type);
bool CheckNext(std::list<Token>::iterator it, TokenType expected_type);
bool CheckOnOf(const TokenType& token, const std::initializer_list<TokenType>& types);
bool CheckOnOf(const Token& token, const std::initializer_list<TokenType>& types);