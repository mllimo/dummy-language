#include "buildin.h"

#include <iostream>

void Print(Object& obj) {
    if (obj.GetType() == Object::Type::NUMBER) {
        std::cout << obj.Get<PrimitiveNumber>();
    }
    else if (obj.GetType() == Object::Type::STRING) {
        std::cout << obj.Get<std::string>();
    }
}

void Scan(Object& obj) {
    if (obj.GetType() == Object::Type::NUMBER) {
        std::cin >> obj.Get<PrimitiveNumber>();
    }
    else if (obj.GetType() == Object::Type::STRING) {
        std::cin >> obj.Get<std::string>();
    }
}