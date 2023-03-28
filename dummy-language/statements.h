#pragma once

#include <list>

#include "expressions.h"
#include "checkers.h"

enum StatementType {
    StatementType_UNDEFINED,
    DECLARATION,
    EXPRESSION
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

    Declaration(Type type, const std::string& id, std::unique_ptr<Expression> expression) :
        type(type),
        id(id),
        expression(std::move(expression))
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
    std::unique_ptr<Expression> expression;
};

class ExpressionStatement : public Statement {
public:
    ExpressionStatement(std::unique_ptr<Expression> expression) :
        expression(std::move(expression))
    {
    }

    StatementType TypeOf() override { return StatementType::EXPRESSION; }

    std::unique_ptr<Expression> expression;
};

class Program {
public:
    std::list<std::unique_ptr<Statement>> statements;
};

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
    // Parse expression
    else if (token.type == TokenType::ID || token.type == TokenType::NUMBER || token.type == TokenType::STRING || token.type == TokenType::L_PARENTHESIS) {
        statement = std::make_unique<ExpressionStatement>(ParseExpresion(current_token));
        ++current_token;
        if (current_token->type != TokenType::SEMICOLON) throw std::runtime_error("Expected semicolon");
    }

    return statement;
}