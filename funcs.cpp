#include "funcs.hpp"
#include "errors.hpp"

std::ostream& operator << (std::ostream& out, const Variable& var) {
	out << "Variable:\nname: " << var.get_name() << "\nvalue: " << var.get_value() << "\n";
	return out;
}

std::ostream& operator << (std::ostream& out, Function& func) {
	out << "Function:\nname: " << func.get_name() << "\nargs: ";
	for (size_t i = 0;i < func.get_args().size();++i) {
		out << func.get_args()[i] << " ";
	}
	out << "\nexpression: " << func.get_expression() << "\nargs_num: " << func.get_args_num() << "\n";
	return out;
}

std::ostream& operator << (std::ostream& out, Token& tok) {
	out << "Token:\n" << "value: " << tok.get_name() << "\n";
	return out;
}


template <typename T>
std::ostream& operator << (std::ostream& out, std::vector<T>& v) {
	for (size_t i = 0;i < v.size();++i) {
		out << v[i] << " ";
	}
	out << "\n";
	return out;
}

template <typename T1, typename T2>
std::ostream& operator << (std::ostream& out, std::map<T1, T2>& m) {
	for (auto pair : m) {
		out << pair.first << " " << pair.second;
	}
	out << "\n";
	return out;
}

void print_format(double d) {
	if (d / 1 == d) {
		std::cout << d << "\n";
	}
	else {
		std::cout << int(d) << "\n";
	}
}

double to_digit(const std::string& s) {
	double res = 0;
	double expr = 0.1;
	int p = s.size();
	if (s.find('.') != s.npos) {
		p = s.find('.');
	}
	for (int i = p + 1;i < s.size();++i) {
		if (!isdigit(s[i])) {
			throw NumberError("invalid character in number notation");
		}
		res += (s[i] - '0') * expr;
		expr /= 10;
	}
	expr = 1;
	for (int i = p - 1;i >= 0;--i) {
		if (!isdigit(s[i])) {
			throw NumberError("invalid character in number notation");
		}
		res += (s[i] - '0') * expr;
		expr *= 10;
	}
	return res;
}

enum symbol_type {
	digit,
	letter,
	opening_bracket,
	closing_bracket,
	comma,
	point,
	oper,
	underscore,
	space,
	undefined
};

symbol_type get_type(char c) {
	std::string operators = "+-*/=%^";
	if (isdigit(c)) {
		return digit;
	}
	else if (isalpha(c)) {
		return letter;
	}
	else if (c == '(') {
		return opening_bracket;
	}
	else if (c == ')') {
		return closing_bracket;
	}
	else if (c == ',') {
		return comma;
	}
	else if (c == '.') {
		return point;
	}
	else if (operators.find(c) != operators.npos) {
		return oper;
	}
	else if (c == '_') {
		return underscore;
	}
	else if (c == ' ') {
		return space;
	}
	else {
		return undefined;
	}
}

int get_prior(const Token& tok) {
	if (tok.get_arity() == tok.unary) {
		return 5;
	}
	else if (tok.get_name() == "%") {
		return 4;
	}
	else if (tok.get_name() == "^") {
		return 3;
	}
	else if (tok.get_name() == "*" || tok.get_name() == "/") {
		return 2;
	}
	else if (tok.get_name() == "+" || tok.get_name() == "-") {
		return 1;
	}
	else {
		return -1;
	}
}

bool part_of_var(char c) {
	symbol_type t = get_type(c);
	return (t == letter || t == digit || t == underscore);
}

bool might_be_var(std::string& s) {
	if (s[0] == '_' || isdigit(s[0])) {
		return false;
	}
	for (auto symb : s) {
		if (!part_of_var(symb)) {
			return false;
		}
	}
	return (!s.empty() && true);
}

bool might_be_digit(std::string& s) {
	for (auto symb : s) {
		if (!(isdigit(symb) || symb=='.' || symb == '+' || symb=='-')) {
			return false;
		}
	}
	return (!s.empty() && true);
}

std::vector<Token> Tokenizator(const std::string& s) {
	std::vector<Token> res = {};
	std::string str = "";
	for (size_t i = 0;i < s.size();++i) {
		symbol_type t = get_type(s[i]);
		if (t == oper) {
			if (might_be_digit(str)) {
				res.push_back(Token(str, Token::digit));
			}
			else if (might_be_var(str)) {
				res.push_back(Token(str, Token::variable));
			}
			else if (!str.empty()) {
				throw ExpressionError("invalid expression");
			}
			str.clear();
			str.push_back(s[i]);
			if (i == 0 || get_type(s[i - 1]) == opening_bracket) {
				if (s[i] == '-') {
					res.push_back(Token(str, Token::oper, Token::no_assoc, Token::unary));
				}
			}
			else if (s[i] == '=' || s[i] == '^') {
				res.push_back(Token(str, Token::oper, Token::right, Token::binary));
			}
			else {
				res.push_back(Token(str, Token::oper, Token::left, Token::binary));
			}
			str.clear();
		}
		else if (t == digit || t == letter || t == underscore) {
			str.push_back(s[i]);
		}
		else if (t == opening_bracket) {
			if (might_be_var(str)) {
				res.push_back(Token(str, Token::func));
			}
			else if (!str.empty()) {
				throw ExpressionError("invalid expression");
			}
			str.clear();
			str.push_back(s[i]);
			res.push_back(Token(str, Token::opening_bracket));
			str.clear();
		}
		else if (t == closing_bracket) {
			if (might_be_digit(str)) {
				res.push_back(Token(str, Token::digit));
			}
			else if (might_be_var(str)) {
				res.push_back(Token(str, Token::variable));
			}
			else if (!str.empty()) {
				throw ExpressionError("invalid expression");
			}
			str.clear();
			str.push_back(s[i]);
			res.push_back(Token(str, Token::closing_bracket));
			str.clear();
		}
		else if (t == comma) {
			if (might_be_digit(str)) {
				res.push_back(Token(str, Token::digit));
			}
			else if (might_be_var(str)) {
				res.push_back(Token(str, Token::variable));
			}
			else if (!str.empty()) {
				throw ExpressionError("invalid expression");
			}
			str.clear();
			str.push_back(s[i]);
			res.push_back(Token(str, Token::comma));
			str.clear();
		}
		else if (t == underscore) {
			continue;
		}
		else if (t == space) {
			continue;
		}
		else {
			throw ExpressionError("undefined symbol in expression");
		}
	}
	if (might_be_digit(str)) {
		res.push_back(Token(str, Token::digit));
	}
	else if (might_be_var(str)) {
		res.push_back(Token(str, Token::variable));
	}
	else if (!str.empty()){
		throw ExpressionError("invalid expression");
	}
	return res;
}

std::vector<Token> Shunting_yard(const std::vector<Token>& expr) {
	std::vector<Token> res = {};
	std::vector<Token> stack = {};
	for (size_t i = 0;i < expr.size();++i) {
		Token::Type t = expr[i].get_type();
		//std::cout << "stack: " << stack << "res: " << res;
		if (t == expr[i].digit) {
			res.push_back(expr[i]);
		}
		else if (t == expr[i].variable) {
			res.push_back(expr[i]);
		}
		else if (t == expr[i].func) {
			//std::cout << "FUNCTION\n";
			stack.push_back(expr[i]);
		}
		else if (t == expr[i].comma) {
			for (int j = stack.size() - 1;j >= 0;--j) {
				if (stack[j].get_type() == stack[j].opening_bracket) {
					break;
				}
				res.push_back(stack[j]);
				stack.pop_back();
			}
		}
		else if (t == expr[i].oper) {
			for (int j = stack.size() - 1;j >= 0;--j) {
				if (get_prior(stack[j]) > get_prior(expr[i]) || (get_prior(stack[j]) == get_prior(expr[i]) && expr[i].get_assoc() == expr[i].left)) {
					res.push_back(stack[j]);
					stack.pop_back();
				}
				else {
					break;
				}
			}
			stack.push_back(expr[i]);
		}
		else if (t == expr[i].opening_bracket) {
			stack.push_back(expr[i]);
		}
		else if (t == expr[i].closing_bracket) {
			for (int j = stack.size() - 1;j >= 0;--j) {
				if (stack[j].get_type() == stack[j].opening_bracket) {
					stack.pop_back();
					if (stack.size() > 0 && stack[j - 1].get_type() == stack[j - 1].func) {
						res.push_back(stack[j - 1]);
						stack.pop_back();
					}
					break;
				}
				else {
					res.push_back(stack[j]);
					stack.pop_back();
				}
			}
		}
		else {
			throw ExpressionError("undefined symbol in expression");
		}
	}
	while (!stack.empty()) {
		res.push_back(stack[stack.size() - 1]);
		stack.pop_back();
	}
	//std::cout << "reversed Polich Notation: " << res << "\n";
	return res;
}

void count_oper(std::vector<double>& stack, const Token& oper) {
	std::string name = oper.get_name();
	if (name == "+") {
		stack[stack.size() - 2] = stack[stack.size() - 2] + stack[stack.size() - 1];
		stack.pop_back();
	}
	else if (name == "-") {
		if (oper.get_arity() == Token::binary) {
			stack[stack.size() - 2] = stack[stack.size() - 2] - stack[stack.size() - 1];
			stack.pop_back();
		}
		else {
			stack[stack.size() - 1] *= -1;
		}
	}
	else if (name == "*") {
		stack[stack.size() - 2] = stack[stack.size() - 2] * stack[stack.size() - 1];
		stack.pop_back();
	}
	else if (name == "/") {
		if (stack[stack.size() - 1] == 0) {
			throw MathError("Error: divided by zero");
		}
		stack[stack.size() - 2] = stack[stack.size() - 2] / stack[stack.size() - 1];
		stack.pop_back();
	}
	else if (name == "%") {
		if (stack[stack.size() - 1] == 0) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 2] = fmod(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
	}
	else if (name == "^") {
		stack[stack.size() - 2] = std::pow(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
	}
}

bool count_func(std::vector<double>& stack, const Token& tok) {
	std::string name = tok.get_name();
	/*Trigonometric functions*/

	if (name == "sin") {
		stack[stack.size() - 1] = sin(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "cos") {
		stack[stack.size() - 1] = cos(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "tg") {
		if (cos(stack[stack.size() - 1]) == 0) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 1] = tan(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "ctg") {
		if (tan(stack[stack.size() - 1]) == 0) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 1] = 1 / tan(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arcsin") {
		stack[stack.size() - 1] = asin(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arccos") {
		stack[stack.size() - 1] = acos(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arctg") {
		stack[stack.size() - 1] = atan(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arcctg") {
		stack[stack.size() - 1] = M_PI / 2 - atan(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arctg2") {
		if (stack[stack.size() - 1] == 0) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 2] = atan2(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*Hyperbolic functions*/

	else if (name == "sh") {
		stack[stack.size() - 1] = sinh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "ch") {
		stack[stack.size() - 1] = cosh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "th") {
		if (cosh(stack[stack.size() - 1] == 0)) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 1] = tanh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "cth") {
		if (sinh(stack[stack.size() - 1] == 0)) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 1] = 1 / tanh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "sch") {
		stack[stack.size() - 1] = 1 / cosh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "csch") {
		if (sinh(stack[stack.size() - 1] == 0)) {
			throw MathError("Error: divided by zero");

		}
		stack[stack.size() - 1] = 1 / sinh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arcsh") {
		stack[stack.size() - 1] = asinh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arcch") {
		stack[stack.size() - 1] = acosh(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "arcth") {
		stack[stack.size() - 1] = atanh(stack[stack.size() - 1]);
		return 1;
	}


	/*exp*/

	else if (name == "exp") {
		stack[stack.size() - 1] = exp(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "frexp") {
		int n = 0;
		stack[stack.size() - 1] = frexp(stack[stack.size() - 2], &n);
		stack.push_back((double)n);
		return 1;
	}
	else if (name == "ldexp") {
		stack[stack.size() - 2] = ldexp(stack[stack.size() - 2], (int)stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "ln") {
		if (stack.size() - 1 <= 0) {
			throw FunctionError("invalid argument passed to function");
		}
		stack[stack.size() - 1] = log(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "lg") {
		if (stack.size() - 1 <= 0) {
			throw FunctionError("invalid argument passed to function");
		}
		stack[stack.size() - 1] = log10(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "modf") {
		double intpart = 0;
		stack.push_back(modf(stack[stack.size() - 1], &intpart));
		stack[stack.size() - 2] = intpart;
		return 1;
	}
	else if (name == "exp2") {
		stack[stack.size() - 1] = exp2(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "expm1") {
		stack[stack.size() - 1] = expm1(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "ilogb") {
		stack[stack.size() - 1] = (double)ilogb(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "log2") {
		if (stack.size() - 1 <= 0) {
			throw FunctionError("invalid argument passed to function");
		}
		stack[stack.size() - 1] = log2(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "scalbn") {
		stack[stack.size() - 2] = scalbn(stack[stack.size() - 2], (int)stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*pow*/

	else if (name == "pow") {
		stack[stack.size() - 2] = pow(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "sqrt") {
		if (stack.size() - 1 <= 0) {
			throw FunctionError("invalid argument passed to function");
		}
		stack[stack.size() - 1] = sqrt(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "cbrt") {
		stack[stack.size() - 1] = cbrt(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "hypot") {
		if (stack.size() - 1 <= 0 || stack.size()-2 <= 0) {
			throw FunctionError("invalid argument passed to function");
		}
		stack[stack.size() - 2] = hypot(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*error and gamma*/

	else if (name == "erf") {
		stack[stack.size() - 1] = erf(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "erfc") {
		stack[stack.size() - 1] = erfc(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "tgamma") {
		stack[stack.size() - 1] = tgamma(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "lgamma") {
		stack[stack.size() - 1] = lgamma(stack[stack.size() - 1]);
		return 1;
	}


	/*rounder and remainer*/

	else if (name == "ceil") {
		stack[stack.size() - 1] = ceil(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "floor") {
		stack[stack.size() - 1] = floor(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "trunc") {
		stack[stack.size() - 1] = trunc(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "round") {
		stack[stack.size() - 1] = round(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "rint") {
		stack[stack.size() - 1] = rint(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "nearbyint") {
		stack[stack.size() - 1] = nearbyint(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "remainder") {
		stack[stack.size() - 2] = remainder(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*floating-point manipulation*/

	else if (name == "copysign") {
		stack[stack.size() - 2] = copysign(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "nextafter") {
		stack[stack.size() - 2] = nextafter(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "nextafter") {
		stack[stack.size() - 2] = nextafter(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*min max dif functions*/

	else if (name == "fdim") {
		stack[stack.size() - 2] = fdim(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "min") {
		stack[stack.size() - 2] = fmin(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}
	else if (name == "max") {
		stack[stack.size() - 2] = fmax(stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		return 1;
	}


	/*abs and multiply-add*/

	else if (name == "abs") {
		stack[stack.size() - 1] = fabs(stack[stack.size() - 1]);
		return 1;
	}
	else if (name == "fma") {
		stack[stack.size() - 3] = fma(stack[stack.size() - 3], stack[stack.size() - 2], stack[stack.size() - 1]);
		stack.pop_back();
		stack.pop_back();
		return 1;
	}
	return 0;
}


double Evaluate(const std::vector<Token>& expr, std::map<std::string, Variable>& vars, std::map<std::string, Function>& funcs) {
	std::vector<double> stack = {};
	for (size_t i = 0;i < expr.size();++i) {
		//std::cout << stack;
		if (expr[i].get_type() == expr[i].digit) {
			stack.push_back(to_digit(expr[i].get_name()));
		}
		else if (expr[i].get_type() == expr[i].oper) {
			count_oper(stack, expr[i]);
		}	
		else if (expr[i].get_type() == expr[i].func) {
			if (!count_func(stack, expr[i])) {
				if (!funcs.count(expr[i].get_name())) {
					throw FunctionError("function not found");
				}
				std::vector<std::string> args = {};
				int args_num = funcs[expr[i].get_name()].get_args_num();
				if (i < args_num) {
					throw FunctionError("incorrect number of args passed to function");
				}
				for (int j = i - args_num;j < i;++j) {
					args.push_back(expr[j].get_name());
				}
				if (args_num != args.size()) {
					throw FunctionError("incorrect number of args passed to function");
				}
				else {
					//std::cout << "args: " << args;
					std::map<std::string, Variable> local_vars = {};
					std::vector<std::string> func_args = funcs[expr[i].get_name()].get_args();
					for (int j = 0;j < args.size();++j) {
						local_vars[func_args[j]] = Variable(func_args[j], Evaluate(Shunting_yard(Tokenizator(args[j])), vars, funcs));
					}
					//std::cout << local_vars;
					std::string expression = funcs[expr[i].get_name()].get_expression();
					stack[stack.size() - args.size()] = (Evaluate(Shunting_yard(Tokenizator(expression)), local_vars, funcs));
					for (int i = 0;i < args.size() - 1;++i) {
						stack.pop_back();
					}
				}
			}
		}
		else if (expr[i].get_type() == expr[i].variable) {
			if (!vars.count(expr[i].get_name())) {
				throw VariableError("variable not found");
			}
			else {
				stack.push_back(vars[expr[i].get_name()].get_value());
			}
		}
	}
	if (stack.size() > 1) {
		throw ExpressionError("invalid expression");
	}
	return stack[0];
}

void func_parser(const std::string& s, std::map<std::string, Function>& funcs) {
	std::string name = "";
	std::string arg = "";
	std::vector<std::string> args = {};
	std::string expression = "";
	int flag = 0;
	for (size_t i = 0;i < s.size();++i) {
		if (s[i] == '=') {
			flag = 1;
		}
		else if (s[i] == '(' && flag == 0) {
			flag = 2;
		}
		else if (s[i] == ' ') {
			continue;
		}
		else if (s[i] == ',' || s[i] == ')' && flag == 2) {
			args.push_back(arg);
			arg.clear();
		}
		else {
			if (!flag) {
				name.push_back(s[i]);
			}
			else if (flag == 1) {
				expression.push_back(s[i]);
			}
			else if (flag == 2) {
				arg.push_back(s[i]);
			}
		}
	}
	funcs[name] = Function(name, args, expression, args.size());
	//std::cout << funcs[name];
}

void var_parser(const std::string& s, std::map<std::string, Variable>& vars, std::map<std::string, Function>& funcs) {
	std::string name = "";
	std::string value = "";
	int flag = 0;
	for (size_t i = 0;i < s.size();++i) {
		if (s[i] == '=') {
			flag = 1;
		}
		else if (s[i] == ' ') {
			continue;
		}
		else {
			if (!flag) {
				name.push_back(s[i]);
			}
			else {
				value.push_back(s[i]);
			}
		}
	}
	try {
		vars[name] = Variable(name, Evaluate(Shunting_yard(Tokenizator(value)), vars, funcs));
	}
	catch(std::string message){
		std::cerr << message << "\n";
	}
	//std::cout << vars[name];
}