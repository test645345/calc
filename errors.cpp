#include "errors.hpp"

MathError::MathError() = default;
MathError::MathError(std::string message) : message(message) {}
std::string MathError::get_message() {
	return message;
}


ExpressionError::ExpressionError() = default;
ExpressionError::ExpressionError(std::string message) : message(message) {}
std::string ExpressionError::get_message() {
	return message;
}


NumberError::NumberError() = default;
NumberError::NumberError(std::string message) : message(message) {}
std::string NumberError::get_message() {
	return message;
}

FunctionError::FunctionError() = default;
FunctionError::FunctionError(std::string message) : message(message) {}
std::string FunctionError::get_message() {
	return message;
}

VariableError::VariableError() = default;
VariableError::VariableError(std::string message) : message(message) {}
std::string VariableError::get_message() {
	return message;
}