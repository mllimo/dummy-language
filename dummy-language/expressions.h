#pragma once

#include <string>
#include <memory>

#include "checkers.h"

enum ExpresionType {
    ExpresionType_UNDEFINED,
    NUMBER,
    STRING,
    ID,
    BINARY_OP
};

class Expression {
public:
    virtual ExpresionType TypeOf() { return {}; }
};

class Number : public Expression {
public:
    double value;

    Number(double value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::NUMBER; }
};

class String : public Expression {
public:
    std::string value;

    String(const std::string& value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::STRING; }
};

class Identification : public Expression {
public:
    std::string value;

    Identification(const std::string& value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::ID; }
};

class BinaryOp : public Expression {
public:
    std::string operator_str;
    std::unique_ptr<Expression> op1;
    std::unique_ptr<Expression> op2;

    BinaryOp(const std::string operator_str, std::unique_ptr<Expression> op1, std::unique_ptr<Expression> op2) :
        operator_str(operator_str),
        op1(std::move(op1)),
        op2(std::move(op2))
    {
    }

    virtual ExpresionType TypeOf() override { return ExpresionType::BINARY_OP; }
};

std::unique_ptr<Expression> ParseExpresion(std::list<Token>::iterator& current_token)
{
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
            expr = std::make_unique<Identification>(current_token->body);
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