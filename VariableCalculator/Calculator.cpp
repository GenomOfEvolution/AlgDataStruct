#include "Calculator.h"

Calculator::Calculator() {	}

float Calculator::Calculate(MyStack<std::string>& postfix)
{
	MyStack<float> stack;

	while (!postfix.isEmpty())
	{
		std::string token = postfix.pop();

		if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") 
		{
			float op2 = stack.pop();
			float op1 = stack.pop();
			if (token == "+") stack.push(op1 + op2);
			else if (token == "-") stack.push(op1 - op2);
			else if (token == "*") stack.push(op1 * op2);
			else if (token == "/") stack.push(op1 / op2);
			else if (token == "^") stack.push(std::pow(op1, op2));
		}
		else if (token == "SIN" || token == "COS" || token == "EXP" || token == "~") 
		{
			float op = stack.pop();
			if (token == "SIN") stack.push(std::sin(op));
			else if (token == "COS") stack.push(std::cos(op));
			else if (token == "EXP") stack.push(std::exp(op));
			else if (token == "~") stack.push(-op);
		}
		else 
		{
			if (token.length() == 1 && token[0] >= 'a' && token[0] <= 'z')
			{
				stack.push(variables[token[0]]);
			}
			else
			{
				stack.push(std::stof(token));
			}
		}
	}

	return stack.pop();
}

bool Calculator::IsOper(char ch)
{
	const char opers[] = { '+', '-', '*', '/', '^', '~' };
	for (auto i : opers)
	{
		if (ch == i)
			return true;
	}
	return false;
}

bool Calculator::IsNumber(const std::string token)
{
	return (token.length() > 0 && token[0] >= '0' && token[0] <= '9');
}

bool Calculator::CheckOper(const std::string& oper)
{
	if (oper.length() == 1)
	{
		if (oper[0] >= 'a' && oper[0] <= 'z')
		{
			auto res = variables.find(oper[0]);
			if (res == variables.end())
			{
				EnterVariable(oper[0]);
			}

			return true;
		}
		return false;
	}

	std::string upOper = oper;
	ToUpperCase(upOper);
	return (upOper == "SIN") || (upOper == "COS") || (upOper == "EXP");
}

void Calculator::ToUpperCase(std::string& str)
{
	for (char& c : str)
	{
		c = std::toupper(c);
	}
}

void Calculator::ParseToken(const std::string& token, MyStack<std::string>& stack)
{
	std::string num = "";
	std::string oper = "";

	for (char ch : token)
	{
		if (IsOper(ch))
		{
			stack.push({ ch });
			continue;
		}

		if (ch >= '0' && ch <= '9')
		{
			if (oper != "")
			{
				if (!CheckOper(oper))
				{
					throw std::runtime_error("Неизвестный оператор: " + oper);
				}

				stack.push(oper);
			}
			oper = "";
			num += ch;
		}
		else
		{
			if (num != "")
			{
				stack.push(num);
			}
			num = "";
			oper += ch;
		}
	}

	if (oper != "")
	{
		if (!CheckOper(oper))
		{
			throw std::runtime_error("Неизвестный оператор: " + oper);
		}
		stack.push(oper);
	}

	if (num != "")
	{
		stack.push(num);
	}
}

void Calculator::EnterVariable(char ch)
{
	int ans;
	while (true)
	{
		std::cout << "Введите значение для " << ch << ": ";
		if (std::cin >> ans)
			break;
		else
		{
			std::cout << "Ошибка: введено не целое число. Попробуйте снова.\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	variables[ch] = ans;
}

void Calculator::ReverseStack(MyStack<std::string>& from, MyStack<std::string>& to)
{
	while (!from.isEmpty())
	{
		to.push(from.pop());
	}
}

std::string Calculator::PostfixToInfix(MyStack<std::string>& tokens)
{
	MyStack<std::pair<std::string, int>> stack;
	while (!tokens.isEmpty())
	{
		std::string token = tokens.pop();
		if (!(token.length() == 1 && token[0] >= 'a' && token[0] <= 'z'))
		{
			ToUpperCase(token);
		}

		auto it = operatorPriority.find(token);

		// Нашли переменную
		if (it == operatorPriority.end())
		{
			stack.push({ token, 6 });
			continue;
		}

		int priority = it->second;
		if (token == "SIN" || token == "COS" || token == "EXP")
		{
			auto op = stack.pop();
			stack.push({ token + "(" + op.first + ")", priority });
			continue;
		}

		if (token == "~") // Унарный минус
		{
			auto nextToken = stack.pop();
			std::string modifiedToken;

			if (nextToken.second == 6)
			{
				modifiedToken = "-" + nextToken.first;
			}
			else
			{
				modifiedToken = "-(" + nextToken.first + ")";
			}

			// Приоритет у итогового выражения как у унарного минуса.
			stack.push({ modifiedToken, operatorPriority.at("~") });
		}

		if (token == "-" || token == "*" || token == "+" || token == "/" || token == "^")
		{
			// Правый операнд
			auto op2 = stack.pop();
			// Левый операнд
			auto op1 = stack.pop();

			std::string leftExpr = op1.first;
			std::string rightExpr = op2.first;

			if (op1.second <= priority && !(op1.second == 6 && priority < 6))
			{
				leftExpr = "(" + op1.first + ")";
			}

			bool wereBracketsPlaced = false;
			if (op2.second < priority || (op2.second == priority && token == "^"))
			{
				wereBracketsPlaced = true;
				rightExpr = "(" + op2.first + ")";
			}
			
			if ((token == "/" || token == "-") && op2.second != 6)
			{
				wereBracketsPlaced = true;
				rightExpr = "(" + op2.first + ")";
			}

			if (!wereBracketsPlaced && op2.second != 6 && rightExpr[0] == '-')
			{
				rightExpr = "(" + rightExpr.substr(0, 2) + ")" + rightExpr.substr(2, rightExpr.size());
			}

			stack.push({ leftExpr + " " + token + " " + rightExpr, priority});
		}
	}

	return stack.pop().first;
}

std::string Calculator::AddBracketsIfNeeded(const std::string& expr, int outerPriority, int innerPriority)
{
	if (outerPriority >= innerPriority)
	{
		return "(" + expr + ")";
	}

	return expr;
}

std::string Calculator::Calc(std::ifstream& input)
{
	MyStack<std::string> revTokens;
	MyStack<std::string> tokens;
	MyStack<std::string> tokens2;
	while (input)
	{
		std::string token;
		input >> token;
		ParseToken(token, revTokens);
	}

	revTokens.Copy(tokens2);
	ReverseStack(revTokens, tokens);

	std::string res;
	std::stringstream ss;
	ss << Calculate(tokens2);


	res +=  PostfixToInfix(tokens) + " = " + ss.str();
	std::cout << res << "\n\n";
	/*if (variables.size() > 0)
	{
		std::cout << ", \n";
		for (auto i : variables)
		{
			std::cout << i.first << " = " << i.second << "\n";
		}
		std::cout << "\n";
	}*/

	return res;
}
