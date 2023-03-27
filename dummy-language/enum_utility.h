#pragma once

#include "token.h"

#include <unordered_map>
#include <stdexcept>
#include <string>

template <typename ENUM>
std::string EnumToString(ENUM value);

template <>
std::string EnumToString<TokenType>(TokenType value);