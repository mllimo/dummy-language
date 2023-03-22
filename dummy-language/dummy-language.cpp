
#include <iostream>
#include <list>
#include <new>

#include "token.h"
#include "enum_utility.h"
#include "lexer.h"
#include "expressions.h"

/*
*
    DECLARATION: TYPE ID | TYPE ID = EXPRESION
    BINARY_OPERATION: LITERAL OP LITERAL
    EXPRESION: LITERAL | BINARY_OPERATION
*/

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

enum StatementType {
    StatementType_UNDEFINED,
    DECLARATION
};

class Statement {
public:
    virtual ~Statement() {}
    virtual StatementType TypeOf() { return {}; }
};

class Declaration : public Statement {
public:
    enum Type {
        UNDEFINED,
        NUMBER,
        STRING
    };

    Declaration(Type type, const std::string& id, std::unique_ptr<Expression> Expression) :
        type(type),
        id(id),
        Expression(std::move(Expression))
    {
    }

    Declaration(Type type, const std::string& id) :
        type(type),
        id(id)
    {
    }

    StatementType TypeOf() override { return StatementType::DECLARATION; }

    Type type;
    std::string id;
    std::unique_ptr<Expression> Expression;
};

class Program {
public:
    std::list<std::unique_ptr<Statement>> statements;
};

std::unique_ptr<Expression> ParseExpresion(std::list<Token>::iterator& current_token)
{
    // TODO
    std::unique_ptr<Expression> expr;
    if (current_token->type == TokenType::NUMBER || current_token->type == TokenType::STRING || current_token->type == TokenType::ID) {

        switch (current_token->type) {
        case TokenType::NUMBER:
            expr = std::make_unique<Number>(stod(current_token->body));
            break;
        case TokenType::STRING:
            expr = std::make_unique<String>(current_token->body);
            break;
        case TokenType::ID:
            expr = std::make_unique<Identifiation>(current_token->body);
            break;
        }

        if (CheckNext(current_token, TokenType::BINARY_OPERATOR)) {
            ++current_token; // Binary operator
            Token token_op = *current_token;
            ++current_token; // second expr
            auto expr2 = ParseExpresion(current_token);
            expr = std::make_unique<BinaryOp>(token_op.body, std::move(expr), std::move(expr2));
        }
    }

    return expr;
}

std::unique_ptr<Statement> ParseDeclaration(std::list<Token>::iterator& current_token)
{
    std::unique_ptr<Declaration> declaration;
    Declaration::Type type;
    std::string id;

    CheckToken(*current_token, TokenType::TYPE);

    if (current_token->body == "number") {
        type = Declaration::Type::NUMBER;
    }
    else if (current_token->body == "string") {
        type = Declaration::Type::STRING;
    }

    ++current_token;

    CheckToken(*current_token, TokenType::ID);

    id = current_token->body;

    if (!CheckNext(current_token, TokenType::BINARY_OPERATOR)) {
        declaration = std::make_unique<Declaration>(type, id);
    }
    else {
        ++current_token; // binary op 
        ++current_token;
        auto expr = ParseExpresion(current_token);
        declaration = std::make_unique<Declaration>(type, id, std::move(expr));
    }

    return declaration;
}

std::unique_ptr<Statement> ParseStatement(std::list<Token>::iterator& current_token)
{
    std::unique_ptr<Statement> statement;
    Token& token = *current_token;

    // Declaration
    if (token.type == TokenType::TYPE) {
        statement = ParseDeclaration(current_token);
        ++current_token;
        if (current_token->type != TokenType::SEMICOLON) throw std::runtime_error("Expected semicolon");
    }

    return statement;
}

Program Parse(std::list<Token>& tokens)
{
    Program program;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        auto statement = ParseStatement(it);
        if (statement == nullptr) break;
        program.statements.push_back(std::move(statement));
    }
    return program;
}

int main()
{

    std::string text = "number variable = 2;";
    auto tokens = Tokenize(text);

    for (auto& token : tokens) {
        std::cout << token.body << "(" << EnumToString(token.type) << ")" << std::endl;
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
