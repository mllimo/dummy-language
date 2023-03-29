#pragma once

#include <string>
#include <memory>
#include <stack>

#include "checkers.h"
#include "scope.h"

class Expression {
public:
    enum class Type {
        UNDEFINED,
        NUMBER,
        STRING,
        ID,
        BINARY_OP,
        F_CALL
    };

    virtual Expression::Type TypeOf() { return {}; }
    virtual bool IsSimpleExpression() { return true; }
    virtual Object Evaluate(Scope& scope) { return {}; }
};

// TODO: change to double :)
class Number : public Expression {
public:
    int value;

    Number(int value) : value(value) {}
    virtual Expression::Type TypeOf() override { return Expression::Type::NUMBER; }
    virtual bool IsSimpleExpression() { return true; }
    Object Evaluate(Scope& scope) override { return Object(value); }
};

class String : public Expression {
public:
    std::string value;

    String(const std::string& value) : value(value) {}
    virtual Expression::Type TypeOf() override { return Expression::Type::STRING; }
    virtual bool IsSimpleExpression() { return true; }
    Object Evaluate(Scope& scope) override { return Object(value); }
};

class Identification : public Expression {
public:
    std::string value;

    Identification(const std::string& value) : value(value) {}
    virtual bool IsSimpleExpression() { return true; }
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

    virtual Expression::Type TypeOf() override { return Expression::Type::BINARY_OP; }
    virtual bool IsSimpleExpression() { return false; }
    Object Evaluate(Scope& scope) override;

private:
    int GetPrecedence(const std::string& op_id);
    void RecursiveEval(Expression& current_expression, Scope& scope, std::stack<Object>& values, std::stack<std::string>& operators);
    Object InfixEval(std::stack<Object>& values, std::stack<std::string>& operators);

};

class FunctionCall : public Expression {
public:
    FunctionCall(const std::string& id, std::unique_ptr<Expression> expresion) :
        id(id),
        args(std::move(expresion))
    {
    }

    Expression::Type TypeOf() override { return Expression::Type::F_CALL; }
    Object Evaluate(Scope& scope) override;


    std::string id;
    std::unique_ptr<Expression> args;
};

// header
std::unique_ptr<Expression> ParseFunctionCall(std::list<Token>::iterator& current_token);
std::unique_ptr<Expression> ParseExpresion(std::list<Token>::iterator& current_token);