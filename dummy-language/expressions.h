#pragma once

#include <string>
#include <memory>

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

class Identifiation : public Expression {
public:
    std::string value;

    Identifiation(const std::string& value) : value(value) {}
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
