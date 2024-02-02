#include "token.hpp"

Token::Token() = default;
Token::Token(std::string str, Type type, Assoc assoc, Arity arity) : name(str), type(type), assoc(assoc), arity(arity) {}
std::string Token::get_name() const {
	return name;
}
Token::Type Token::get_type() const {
	return type;
}
Token::Assoc Token::get_assoc() const {
	return assoc;
}
Token::Arity Token::get_arity() const {
	return arity;
}