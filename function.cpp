#include "function.hpp"

Function::Function() = default;
Function::Function(std::string name, std::vector<std::string> args, std::string expression, int args_num) : name(name), args(args), expression(expression), args_num(args_num) {}
std::string Function::get_name() const {
	return name;
}
std::vector<std::string> Function::get_args() const {
	return args;
}
std::string Function::get_expression() const {
	return expression;
}
int Function::get_args_num() const {
	return args_num;
}
std::map<std::string, std::string> Function::match(std::vector<std::string> user_args) const {
	if (user_args.size() != args_num) {
		throw "wrong number of arguments passed to function";
	}
	std::map<std::string, std::string> match = {};
	for (size_t i = 0;i < args_num;++i) {
		match[args[i]] = user_args[i];
	}
	return match;
}