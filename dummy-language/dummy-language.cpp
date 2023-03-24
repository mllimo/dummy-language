
#include <iostream>
#include <list>
#include <new>

#include "token.h"
#include "enum_utility.h"
#include "lexer.h"
#include "expressions.h"
#include "statements.h"
#include "object.h"

// buildin function
void Print(Object& obj) {
    if (obj.GetType() == Object::Type::INT) {
        std::cout << obj.Get<int>();
    }
    else if (obj.GetType() == Object::Type::STRING) {
        std::cout << obj.Get<std::string>();
    }
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

/*
    ARGS["ID"] = VALUE;


void Print(Value& value) {
    if (value->typeof() == int) {
        std::cout << value.asInt();
    }
}

* GLOBAL SCOPE:
*   ID -> VALUE
* 
* VALUE:
*   VALUE -> INT -> 1
*   VALUE -> STRING -> "hola"
*   VALUE -> FUNCTION -> NO RETURN JUST APPLY
* 
* FUNCTION(ARG, GLOBAL SCOPE) { 
*   DO SHIT WITH ARGS
* }
* 
* 
* GLOBAL["print"] = new Function(); 
*/

int main()
{

    std::string text = "print(2+2);";
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
 
/*
number a = 2;
number b = 3;
number c = a + b;

print(c)
*/
