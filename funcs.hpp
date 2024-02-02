#pragma once
#include<iostream>
#include "Token.hpp"
#include "function.hpp"
#include "variable.hpp"
#define M_PI 3.14159265358979323846

template <typename T>
std::ostream& operator << (std::ostream& out, const std::vector<T>& v);
template <typename T1, typename T2>
std::ostream& operator << (std::ostream& out, const std::map<T1, T2>& m);
std::ostream& operator << (std::ostream& out, const Variable& var);
std::ostream& operator << (std::ostream& out, const Function& func);
std::ostream& operator << (std::ostream& out, const Token& tok);
void print_format(double d);


double to_digit(const std::string& s);
enum symbol_type;
symbol_type get_type(char c);
int get_prior(const Token& tok);

bool part_of_var(const char c);

std::vector<Token> Tokenizator(const std::string& s);

std::vector<Token> Shunting_yard(const std::vector<Token>& expr);

void count_oper(std::vector<double>& stack, const Token& oper);
bool count_func(std::vector<double>& stack, const Token& tok);

double Evaluate(const std::vector<Token>& expr, std::map<std::string, Variable>& vars, std::map<std::string, Function>& funcs);

void func_parser(const std::string& s, std::map<std::string, Function>& funcs);
void var_parser(const std::string& s, std::map<std::string, Variable>& vars, std::map<std::string, Function>& funcs);