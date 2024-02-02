#pragma once
#include<string>
#include<vector>
#include<map>

class Function {
private:
	std::string name = "";
	std::vector<std::string> args = {};
	std::string expression = "";
	int args_num = 0;
public:
	Function();
	Function(std::string name, std::vector<std::string> args, std::string expression, int args_num);
	std::string get_name() const;
	std::vector<std::string> get_args() const;
	std::string get_expression() const;
	int get_args_num() const;
	std::map<std::string, std::string> match(std::vector<std::string> user_args) const;
};