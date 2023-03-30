#pragma once

#include "object.h"
#include <memory>
#include <unordered_map>

class Scope {
public:
	Scope() = default;
	Scope(std::unordered_map<std::string, Object> variables) : variables_(std::move(variables)) {}

	void Add(const std::string& name, Object&& value)
	{
		variables_.emplace(name, std::move(value));
	}

	Object& operator[](const std::string& name) {
		auto it = variables_.find(name);
		if (it == variables_.end())
			throw std::runtime_error("Variable " + name + " not in scope");
		return it->second;
	}

private:
	std::unordered_map<std::string, Object> variables_;
};