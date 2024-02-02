#pragma once
#include<string>

class Token {
public:
	enum Type {
		oper,
		opening_bracket,
		closing_bracket,
		func,
		variable,
		digit,
		comma,
		null
	};
	enum Assoc {
		left,
		right,
		no_assoc
	};
	enum Arity {
		unary,
		binary,
		ternary,
		no_arity
	};
	Token();
	Token(std::string str, Type type, Assoc assoc=no_assoc, Arity arity=no_arity);
	std::string get_name() const;
	Type get_type() const;
	Assoc get_assoc() const;
	Arity get_arity() const;
private:
	Type type = null;
	Assoc assoc = no_assoc;
	Arity arity = no_arity;
	std::string name = "";
};