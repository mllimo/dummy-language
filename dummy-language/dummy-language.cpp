
#include <iostream>
#include <list>

#include "token.h"
#include "enum_utility.h"
#include "lexer.h"

/*
* 
    DECLARATION: TYPE ID | TYPE ID = EXPRESION
    BINARY_OPERATION: LITERAL OP LITERAL
    EXPRESION: LITERAL | BINARY_OPERATION
*/

enum StatementType {
    StatementType_UNDEFINED,
    BASE_STATEMENT,
    DECLARATION
};

class Statement {
public:
    virtual ~Statement() {}
    virtual StatementType TypeOf() { return StatementType::BASE_STATEMENT; }
};

class Declaration : public Statement {
public:
    enum Type {
        UNDEFINED,
        NUMBER,
        STRING
    };

    Declaration(Type type, const std::string& id) :
        type_(type),
        id_(id)
    {
    }

    StatementType TypeOf() override { return StatementType::DECLARATION; }

private:
    Type type_;
    std::string id_;
};

class Program {
public:
    std::list<std::unique_ptr<Statement>> statements;
};

std::unique_ptr<Statement> ParseDeclaration(std::list<Token>::iterator& current_token)
{
    Declaration::Type type;
    std::string id;

    // CheckToken(token, type)
    if (current_token->type != TokenType::TYPE) throw std::runtime_error("Declaration with invalid token: Expected token of type TYPE");

    if (current_token->body == "number") {
        type = Declaration::Type::NUMBER;
    }
    else if (current_token->body == "string") {
        type = Declaration::Type::STRING;
    }

    ++current_token;

    // CheckToken(token, type)
    if (current_token->type != TokenType::ID) throw std::runtime_error("Declaration with invalid token: Expected token of type ID");
    id = current_token->body;

    std::unique_ptr<Declaration> declaration = std::make_unique<Declaration>(type, id);
    return declaration;
}

Program Parse(std::list<Token>& tokens)
{
    Program program;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        Token& token = *it;
        if (token.type == TokenType::TYPE) {
            program.statements.push_back(ParseDeclaration(it));
            ++it;
            if (it->type != TokenType::SEMICOLON) throw std::runtime_error("Expected semicolon");
        }
    }
    return program;
}

int main()
{

    std::string text = "number variable;";
    auto tokens = Tokenize(text);

    for (auto& token : tokens) {
        std::cout << token.body  << "(" << EnumToString(token.type) << ")"  << std::endl;
    }

    try {
        Program program = Parse(tokens);
        for (auto& statement : program.statements) {
            std::cout << statement->TypeOf() << std::endl;
        }
    }
    catch (std::exception& err) {
        std::cerr << err.what() << std::endl;
    }


    return 0;
}
