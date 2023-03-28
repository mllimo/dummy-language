#include "expressions.h"

#include <unordered_map>
#include <stdexcept>

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
                expr = std::make_unique<Number>(stoi(current_token->body));
                break;
            case TokenType::STRING:
                expr = std::make_unique<String>(current_token->body);
                break;
            case TokenType::ID:
                expr = std::make_unique<Identification>(current_token->body);
                break;
            }

            if (CheckNext(current_token, TokenType::BINARY_OPERATOR)) {
                ++current_token;
                Token token_op = *current_token;
                ++current_token;
                auto expr2 = ParseExpresion(current_token);
                expr = std::make_unique<BinaryOp>(token_op.body, std::move(expr), std::move(expr2));
            }
        }
    }
    else if (current_token->type == TokenType::L_PARENTHESIS) {
        // TODO: Refactor
        ++current_token;
        expr = ParseExpresion(current_token);
        ++current_token;
        CheckToken(*current_token, TokenType::R_PARENTHESIS);

        if (CheckNext(current_token, TokenType::BINARY_OPERATOR)) {
            ++current_token;
            Token token_op = *current_token;
            ++current_token;
            auto expr2 = ParseExpresion(current_token);
            expr = std::make_unique<BinaryOp>(token_op.body, std::move(expr), std::move(expr2));
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

int BinaryOp::GetPrecedence(const std::string& op_id)
{
    static const std::unordered_map<std::string, int> precedence = {
        {"+", 1},
        {"-", 1},
        {"*", 2},
        {"/", 2}
    };

    auto it = precedence.find(op_id);
    if (it == precedence.end()) throw std::runtime_error("Operator id unknown");
    return it->second;
}

Object BinaryOp::InfixEval(std::stack<Object>& values, std::stack<std::string>& operators)
{
    // TODO: string and erros

    Object x = std::move(values.top());
    values.pop();
    Object y = std::move(values.top());
    values.pop();

    std::string op = std::move(operators.top());
    operators.pop();

    if (op == "+") {
        return  y.Get<PrimitiveNumber>() + x.Get<PrimitiveNumber>();
    }
    else if (op == "-") {
        return y.Get<PrimitiveNumber>() - x.Get<PrimitiveNumber>();
    }
    else if (op == "*") {
        return x.Get<PrimitiveNumber>() * y.Get<PrimitiveNumber>();
    }
    else if (op == "/") {
        if (x.Get<PrimitiveNumber>() == 0) throw std::runtime_error("Division by 0");
        return y.Get<PrimitiveNumber>() / x.Get<PrimitiveNumber>();
    }
   
    throw std::runtime_error("Can not evaluate the current operator: " + op);
    return 0;
}

void BinaryOp::RecursiveEval(Expression& current_expression, Scope& scope, std::stack<Object>& values, std::stack<std::string>& operators)
{
    if (current_expression.TypeOf() == ExpressionType::NUMBER ||
        current_expression.TypeOf() == ExpressionType::STRING ||
        current_expression.TypeOf() == ExpressionType::ID ||
        current_expression.TypeOf() == ExpressionType::F_CALL) {
        values.push(current_expression.Evaluate(scope));
    }
    else if (current_expression.TypeOf() == ExpressionType::BINARY_OP) {
        
        BinaryOp& current_cast = dynamic_cast<BinaryOp&>(current_expression);

        // OP 1 evaluation
        if (current_cast.op1->IsSimpleExpression()) {
            values.push(current_cast.op1->Evaluate(scope));
        }
        else {
            RecursiveEval(*current_cast.op1, scope, values, operators);
            Object result = InfixEval(values, operators);
            values.push(std::move(result));
        }

        while (!operators.empty() && GetPrecedence(current_cast.operator_str) <= GetPrecedence(operators.top())) {
            Object result = InfixEval(values, operators);
            values.push(std::move(result));
        }

        // Add current operator
        operators.push(current_cast.operator_str);

        // OP 2 evaluation
        if (current_cast.op2->IsSimpleExpression()) {
            values.push(current_cast.op2->Evaluate(scope));
        }
        else {
            RecursiveEval(*current_cast.op2, scope, values, operators);
        }

    }
    else {
        throw std::runtime_error("RecursiveEval can not process the current expression");
    }
}

Object BinaryOp::Evaluate(Scope& scope)
{
    std::stack<Object> values;
    std::stack<std::string> operators;
    
    RecursiveEval(*this, scope, values, operators);

    // Evaluate if there is any operator
    while (!operators.empty() && GetPrecedence(operators.top())) {
        Object result = InfixEval(values, operators);
        values.push(std::move(result));
    }

    if (values.size() != 1) throw std::runtime_error("Binary operator evaluation failed");

    return std::move(values.top());
}