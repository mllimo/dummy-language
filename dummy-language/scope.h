#pragma once

#include "object.h"
#include <memory>
#include <unordered_map>

class Scope {
public:
	Scope() = default;
	Scope(std::unordered_map<std::string, std::reference_wrapper<Object>> variables) : variables_(std::move(variables)) {}

	Object& operator[](const std::string& name) {
		auto it = variables_.find(name);
		if (it == variables_.end())
			std::runtime_error("Variable name not in scope");
		return it->second;
	}

private:
	std::unordered_map<std::string, std::reference_wrapper<Object>> variables_;
};