#pragma once
#include "MyStack.h"

#include <utility>
#include <cmath>
#include <map>
#include <fstream>
#include <string>
#include <iostream>

class Calculator
{
public:
	Calculator();
	void Calc(std::ifstream& input);

private:
	std::map<char, int> variables;
	const std::map<std::string, int> operatorPriority = {
		{ "+", 1 },
		{ "-", 1 },
		{ "*", 2 },
		{ "/", 2 },
		{ "^", 3 },
		{ "~", 4 },
		{ "SIN", 5 },
		{ "COS", 5 },
		{ "EXP", 5 },
	};

	float Calculate(MyStack<std::string>& stack);
	bool IsOper(char ch);
	bool IsNumber(const std::string token);
	bool CheckOper(const std::string& oper);
	void ToUpperCase(std::string& str);
	void ParseToken(const std::string& token, MyStack<std::string>& stack);
	void EnterVariable(char ch);
	void ReverseStack(MyStack<std::string>& from, MyStack<std::string>& to);
	std::string PostfixToInfix(MyStack<std::string>& tokens);
	std::string AddBracketsIfNeeded(const std::string& expr, int outerPriority, int innerPriority);
};