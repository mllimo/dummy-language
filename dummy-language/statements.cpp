#include "statements.h"

Program::Program(std::list<Token>& tokens)
{
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        auto statement = ParseStatement(it);
        if (statement == nullptr) break;
        statements.push_back(std::move(statement));
    }
}

void Declaration::Evaluate(Scope& scope)
{
    scope.Add(id, expression->Evaluate(scope));
}

void ExpressionStatement::Evaluate(Scope& scope)
{
    expression->Evaluate(scope);
}

void Program::Evaluate(Scope& scope)
{
    for (auto& statement : statements)
        statement->Evaluate(scope);
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
    // Parse expression
    else if (CheckOnOf(token, { TokenType::ID, TokenType::NUMBER, TokenType::STRING, TokenType::L_PARENTHESIS })) {
        statement = std::make_unique<ExpressionStatement>(ParseExpresion(current_token));
        ++current_token;
        if (current_token->type != TokenType::SEMICOLON) throw std::runtime_error("Expected semicolon");
    }

    return statement;
}