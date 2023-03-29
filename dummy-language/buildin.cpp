#include "buildin.h"

#include <iostream>

void Print(Object& obj) {
    if (obj.GetType() == Object::Type::INT) {
        std::cout << obj.Get<int>();
    }
    else if (obj.GetType() == Object::Type::STRING) {
        std::cout << obj.Get<std::string>();
    }
}

void Scan(Object& obj) {
    if (obj.GetType() == Object::Type::INT) {
        std::cin >> obj.Get<int>();
    }
    else if (obj.GetType() == Object::Type::STRING) {
        std::cin >> obj.Get<std::string>();
    }
}