#pragma once

// CUSTOM PRIMITIVES 2

#include <iostream>
#include <vector>
#include <functional>

typedef int PrimitiveNumber;

class Object {
public:
	using Vector = std::vector<Object>;
	enum class Type { UNDEFINED, INT, BOOL, STRING, VECTOR, CLASS };

	Object() : type_(Type::UNDEFINED) {}

	Object(Object&& object)
	{
		value_ = std::move(object.value_);
		type_ = object.type_;
		object.type_ = Type::UNDEFINED;
	}



	Object(int value) : type_(Type::INT) {
		value_ = new int(value);
	}

	Object(bool value) : type_(Type::BOOL) {
		value_ = new bool(value);
	}

	Object(const std::string& value) : type_(Type::STRING) {
		value_ = new std::string(value);
	}

	Object(std::string&& value) : type_(Type::STRING) {
		value_ = new std::string(std::move(value));
	}

	Object(const char* value) : type_(Type::STRING) {
		value_ = new std::string(value);
	}

	Object(const Object& value) : type_(value.type_) {
		value_ = value.Copy();
	}

	template <typename T>
	Object(const std::initializer_list<T>& value) : type_(Type::VECTOR) {
		value_ = new Vector(value.begin(), value.end());
	}

	~Object() {
		Free();	
	}

	Object& operator=(Object&& object) {
		type_ = object.type_;
		object.type_ = Type::UNDEFINED;
		value_ = object.value_;
		object.value_ = nullptr;
		return *this;
	}

	Object& operator=(const Object& object) {
		Free();
		type_ = object.type_;
		value_ = object.Copy();
		return *this;
	}

	void* Copy() const {
		if (type_ == Type::INT) {
			return new int(*reinterpret_cast<int*>(value_));
		}
		else if (type_ == Type::BOOL) {
			return new bool(*reinterpret_cast<bool*>(value_));
		}
		else if (type_ == Type::STRING) {
			return new std::string(*reinterpret_cast<std::string*>(value_));
		}
		else if (type_ == Type::VECTOR) {
			return new Vector(*reinterpret_cast<Vector*>(value_));
		}
		else if (type_ != Type::UNDEFINED) {
			throw std::runtime_error("Uknown type");
		}
		return nullptr;
	}

	Type GetType() const { return type_; }

	// Cast
	template <typename T>
	const T& Get() const {
		T* ptr = reinterpret_cast<T*>(value_);
		T& ref = *ptr;
		return ref;
	}

	template <typename T>
	T& Get() {
		T* ptr = reinterpret_cast<T*>(value_);
		T& ref = *ptr;
		return ref;
	}
	//

	friend std::ostream& operator<<(std::ostream& os, const Object& object) {
		if (object.type_ == Type::INT) {
			os << object.Get<int>();
		}
		else if (object.type_ == Type::BOOL) {
			os << object.Get<bool>();
		}
		else if (object.type_ == Type::STRING) {
			os << object.Get<std::string>();
		}
		else if (object.type_ == Type::VECTOR) {
			auto& vector = object.Get<Vector>();
			os << "[ ";
			for (auto& value : vector) {
				os << value << ' ';
			}
			os << "]";
		}
		else if (object.type_ != Type::UNDEFINED) {
			throw std::runtime_error("Uknown type");
		}
		return os;
	}

protected:
	Type type_ = Type::UNDEFINED;
	void* value_ = nullptr;

	template <typename T>
	void FreePtr(void*& value)
	{
		T*& ptr = reinterpret_cast<T*&>(value);
		if (ptr) {
			delete ptr;
			ptr = nullptr;
		}
	}

	virtual void Release() {}

	void Free() {
		if (type_ == Type::INT) {
			FreePtr<int>(value_);
		}
		else if (type_ == Type::BOOL) {
			FreePtr<bool>(value_);
		}
		else if (type_ == Type::STRING) {
			FreePtr<std::string>(value_);
		}
		else if (type_ == Type::VECTOR) {
			FreePtr<Vector>(value_);
		}
		else if (type_ == Type::CLASS) {
			Release();
		}
		else if (type_ != Type::UNDEFINED) {
			throw std::runtime_error("Uknown type");
		}
		type_ = Type::UNDEFINED;
	}
};

template <typename CustomClass>
class Class : public Object {
public:
	Class() {
		value_ = new CustomClass;
		type_ = Type::CLASS;
	}

	template <typename... ARGS>
	Class(ARGS&&... args) {
		value_ = new CustomClass(std::move(args...));
		type_ = Type::CLASS;
	}

	virtual void Release() override {
		CustomClass*& ptr = reinterpret_cast<CustomClass*&>(value_);
		if (ptr) {
			delete ptr;
			ptr = nullptr;
		}
	}

	operator CustomClass& () { return *reinterpret_cast<CustomClass*>(value_); }
	operator CustomClass const& () { return *reinterpret_cast<CustomClass*>(value_); }
}; 