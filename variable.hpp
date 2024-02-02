#pragma once
#include<string>

class Variable {
private:
	std::string name = "";
	double value = 0;
public:
	Variable();
	Variable(std::string name, double value);
	double get_value() const;
	std::string get_name() const;
};