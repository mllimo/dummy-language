#include <string>
#include <iostream>
#include <regex>
#include <unordered_map>

enum TokenType 
{
    UNDEFINED,
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

template <typename ENUM>
std::string EnumToString(ENUM value) { return ""; }

template <>
std::string EnumToString<TokenType>(TokenType value)
{
    static const std::unordered_map<TokenType, std::string> map = {
        {TokenType::UNDEFINED, "UNDEFINED"},
        {TokenType::ID, "ID"},
        {TokenType::NUMBER, "NUMBER"},
        {TokenType::STRING, "STRING"},
        {TokenType::COMMENT, "COMMENT"},
        {TokenType::WHITE, "WHITE"},
        {TokenType::TYPE, "TYPE"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::BINARY_OPERATOR, "BINARY_OPERATOR"}
    };

    auto it = map.find(value);
    if (it == map.end()) throw std::runtime_error("EnumToString<TokenType> fails");
    return it->second;
}

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

std::vector<Token> Tokenize(std::string& text) 
{
    std::vector<Token> tokens;
    while (!text.empty()) {
        Token token = Lexer(text);
        tokens.emplace_back(std::move(token));
    }

    return tokens;
}

int main()
{

    std::string text = "number variable\t=\t3 + 3";
    auto tokens = Tokenize(text);

    for (auto& token : tokens) {
        std::cout << token.body  << "(" << EnumToString(token.type) << ")"  << std::endl;
    }

    return 0;
}
