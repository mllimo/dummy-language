
#include <iostream>
#include <list>
#include <new>

#include "token.h"
#include "enum_utility.h"
#include "lexer.h"
#include "expressions.h"
#include "statements.h"
#include "object.h"
#include "buildin.h"

int main()
{
    std::string text = "number a = 2; \
                        number b = 3; \
                        number c = a + b; \
                        print(\"a + b = \"); \
                        print(c);";

    auto tokens = Tokenize(text);

    try {
        Program program(tokens);
        Scope fake_scope;

        // buildin
        auto print_f = Class<std::function<void(Object&)>>(&Print);
        auto scan_f = Class<std::function<void(Object&)>>(&Scan);
        //

        fake_scope.Add("print", std::move(print_f));
        fake_scope.Add("scan", std::move(scan_f));

        program.Evaluate(fake_scope);
    }
    catch (std::exception& err) {
        std::cerr << err.what() << std::endl;
    }


    return 0;
}
