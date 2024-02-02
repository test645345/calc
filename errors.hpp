#pragma once
#include<string>

class MathError {
private:
	std::string message = {};
public:
	MathError();
	MathError(std::string message);
	std::string get_message();
};

class ExpressionError {
private:
	std::string message = {};
public:
	ExpressionError();
	ExpressionError(std::string message);
	std::string get_message();

};

class NumberError {
private:
	std::string message = {};
public:
	NumberError();
	NumberError(std::string message);
	std::string get_message();

};

class FunctionError {
private:
	std::string message = {};
public:
	FunctionError();
	FunctionError(std::string message);
	std::string get_message();

};

class VariableError {
private:
	std::string message = {};
public:
	VariableError();
	VariableError(std::string message);
	std::string get_message();

};