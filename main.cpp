#include "token.hpp"
#include "function.hpp"
#include "variable.hpp"
#include "data.hpp"
#include "funcs.hpp"
#include "errors.hpp"


int main() {
	std::string s = "";
	//std::cout << to_digit(s);
	while (true) {
		std::getline(std::cin, s);
		if (s.find('=') != s.npos) {
			if (s.find('(') != s.npos && s.find('(') < s.find('=')) {
				func_parser(s, funcs);
			}
			else {
				var_parser(s, vars, funcs);
			}
		}
		else if (s.empty()) {
			continue;
		}
		else {
			try {
				print_format(Evaluate(Shunting_yard(Tokenizator(s)), vars, funcs));
			}
			catch(MathError err) {
				std::cerr << "Math error occured: " << err.get_message() << "\n";
			}
			catch (ExpressionError err) {
				std::cerr << "Expression error occured: " << err.get_message() << "\n";
			}
			catch (NumberError err) {
				std::cerr << "Number error error occured: " << err.get_message() << "\n";
			}
			catch (FunctionError err) {
				std::cerr << "Function error occured: " << err.get_message() << "\n";
			}
			catch (VariableError err) {
				std::cerr << "Variable error occured: " << err.get_message() << "\n";
			}
		}
	}
}