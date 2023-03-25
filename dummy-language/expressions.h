#pragma once

#include <string>
#include <memory>

#include "checkers.h"
#include "scope.h"

enum ExpresionType {
    ExpresionType_UNDEFINED,
    NUMBER,
    STRING,
    ID,
    BINARY_OP,
    F_CALL
};

class Expression {
public:
    virtual ExpresionType TypeOf() { return {}; }
    virtual Object Evaluate(Scope& scope) { return {}; }
};

// TODO: change to double :)
class Number : public Expression {
public:
    int value;

    Number(int value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::NUMBER; }
    Object Evaluate(Scope& scope) override { return Object(value); }
};

class String : public Expression {
public:
    std::string value;

    String(const std::string& value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::STRING; }
    Object Evaluate(Scope& scope) override { return Object(value); }
};

class Identification : public Expression {
public:
    std::string value;

    Identification(const std::string& value) : value(value) {}
    virtual ExpresionType TypeOf() override { return ExpresionType::ID; }
    Object Evaluate(Scope& scope) override { return scope[value]; }
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

class FunctionCall : public Expression {
public:
    FunctionCall(const std::string& id, std::unique_ptr<Expression> expresion) :
        id(id),
        args(std::move(expresion))
    {
    }

    ExpresionType TypeOf() override { return ExpresionType::F_CALL; }

    std::string id;
    std::unique_ptr<Expression> args;
};

// header
std::unique_ptr<Expression> ParseFunctionCall(std::list<Token>::iterator& current_token);

std::unique_ptr<Expression> ParseExpresion(std::list<Token>::iterator& current_token)
{
    std::unique_ptr<Expression> expr;
    if (current_token->type == TokenType::NUMBER || current_token->type == TokenType::STRING || current_token->type == TokenType::ID) {

        if (CheckNext(current_token, TokenType::L_PARENTHESIS)) {
            expr = ParseFunctionCall(current_token);
        }
        else {
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
    }

    return expr;
}

std::unique_ptr<Expression> ParseFunctionCall(std::list<Token>::iterator& current_token)
{
    std::unique_ptr<FunctionCall> f_call;
    std::string id;
    std::unique_ptr<Expression> expr_arg;

    CheckToken(*current_token, TokenType::ID);
    id = current_token->body;

    ++current_token;
    CheckToken(*current_token, TokenType::L_PARENTHESIS);

    ++current_token;
    expr_arg = ParseExpresion(current_token);

    if (expr_arg != nullptr) {
        ++current_token;
    }
    CheckToken(*current_token, TokenType::R_PARENTHESIS);

    f_call = std::make_unique<FunctionCall>(id, std::move(expr_arg));
    return f_call;
}