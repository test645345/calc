#include "variable.hpp"

Variable::Variable() = default;
Variable::Variable(std::string name, double value) : name(name), value(value) {}
double Variable::get_value() const {
	return value;
}
std::string Variable::get_name() const {
	return name;
}