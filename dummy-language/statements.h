#pragma once

#include <list>

#include "expressions.h"
#include "checkers.h"

class Statement {
public:
    enum class Type {
        UNDEFINED,
        DECLARATION,
        EXPRESSION
    };

    virtual ~Statement() {}
    virtual Statement::Type TypeOf() { return {}; }
    virtual void Evaluate(Scope& scope) { throw std::runtime_error("Statement::Eval not implemented"); }
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

    Statement::Type TypeOf() override { return Statement::Type::DECLARATION; }
    void Evaluate(Scope& scope) override;

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

    Statement::Type TypeOf() override { return Statement::Type::EXPRESSION; }
    void Evaluate(Scope& scope) override;

    std::unique_ptr<Expression> expression;
};

class Program {
public:
    std::list<std::unique_ptr<Statement>> statements;
    void Evaluate(Scope& scope);
};

std::unique_ptr<Statement> ParseDeclaration(std::list<Token>::iterator& current_token);
std::unique_ptr<Statement> ParseStatement(std::list<Token>::iterator& current_token);