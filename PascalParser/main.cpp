// 26. Программа на ПАСКАЛЕ включает такие  сочетания ключевых
// слов, как    REPEAT..UNTIL, IF..THEN..ELSE, BEGIN..END,
// RECORD..END.Конец оператора  определяется  точкой  с  запятой
// (";").Требуется  проверить  правильность  вложенности данных
// конструкций с учетом допустимости взаимных вложений.В случае
// ошибок указать номер первой некорректной строки(12).

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

#include "MyStack.h"


const std::string keys[] = { "IF", "THEN", "ELSE", "RECORD", "END", "END.", "BEGIN", "REPEAT", "UNTIL" };
const std::string executeKeys[] = { "ELSE", "END", "END.", "UNTIL" };

struct PascalOperator
{
    size_t line = 0;
    std::string key = "";
};

struct ParseFlags
{
    bool isProgramLine = false;
    bool isCondition = false;
    bool isUntilCondition = false;
    bool foundFinalEnd = false;
};

bool IsKey(const std::string& str)
{
    auto it = std::find(std::begin(keys), std::end(keys), str);
    return it != std::end(keys);
}

bool IsExecuteKey(const std::string& str)
{
    auto it = std::find(std::begin(executeKeys), std::end(executeKeys), str);
    return it != std::end(executeKeys);
}

bool IsStringEmptyOrWhitespace(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](unsigned char c) 
        {
        return std::isspace(c);
        });
}

std::string RemoveSpacesBeforeSemicolons(const std::string& input) {
    std::string result = input;
    for (size_t i = result.length(); i > 0; --i) 
    {
        if (result[i] == ';' && i != 0 && result[i - 1] == ' ') 
        {
            result.erase(i - 1, 1);
        }
    }
    return result;
}

std::string CopyLettersUntilNonLetter(const std::string& input)
{
    std::string result;
    for (char c : input)
    {
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '.')
        {
            result += c;
        }
        else
        {
            break;
        }
    }
    return result;
}

std::string FormError(const std::string oper1, PascalOperator oper2)
{
    std::string errorText = "Ожидался оператор \"" + oper1 + "\"" +
        "в строке " + std::to_string(oper2.line) + ", встречено " + oper2.key;
    return errorText;
}

void ExecuteOperators(MyStack<PascalOperator>& operatorStack)
{
    if (operatorStack.count() < 2)
    {
        throw std::runtime_error("Not enough operators to execute");
    }

    // "ELSE", "END", "END.", "UNTIL"
    PascalOperator oper1 = operatorStack.pop();

    // любой другой оператор
    PascalOperator oper2 = operatorStack.pop();

    if (oper2.key == "THEN" && oper1.key != "ELSE")
    {
        PascalOperator ifOper = operatorStack.pop();
        while (oper2.key == "THEN")
        {
            if (ifOper.key == "IF" && !operatorStack.isEmpty())
            {
                oper2 = operatorStack.pop();
            }
            
            if (oper2.key != "THEN")
            {
                break;
            }

            ifOper = operatorStack.pop();

            if (ifOper.key != "IF")
            {
                operatorStack.push(ifOper);
            } 
        }
    }

    if (oper1.key == "THEN")
    {
        if (oper2.key != "IF")
        {
            throw std::runtime_error(FormError("IF", oper2));
        }
        return;
    }

    if (oper1.key == "ELSE")
    {
        if (operatorStack.isEmpty())
        {
            throw std::runtime_error("");
        }

        PascalOperator oper3 = operatorStack.pop();

        if (oper2.key != "THEN")
        {
            throw std::runtime_error(FormError("THEN", oper2));
        }

        if (oper3.key != "IF")
        {
            throw std::runtime_error(FormError("IF", oper3));
        }
        return;
    }

    if (oper1.key == "END.")
    {
        if (oper2.key != "BEGIN")
        {
            throw std::runtime_error(FormError("BEGIN", oper2));
        }
        return;
    }

    if (oper1.key == "END")
    {
        if (oper2.key != "BEGIN" && oper2.key != "RECORD")
        {
            throw std::runtime_error(FormError("BEGIN или RECORD", oper2));
        }
    }

    if (oper1.key == "REPEAT")
    {
        if (oper2.key != "UNTIL")
        {
            throw std::runtime_error(FormError("UNTIL", oper2));
        }
    }
}

bool WorkWithOperators(const size_t linePos, const std::string& str, MyStack<PascalOperator>& operatorStack, ParseFlags flags)
{
    if (IsStringEmptyOrWhitespace(str))
    {
        return true;
    }

    std::istringstream iss(str);
    std::string token;
    while (iss >> token)
    {
        if (token == "PROGRAM")
        {
            flags.isProgramLine = true;
            continue;
        }

        std::string expr = CopyLettersUntilNonLetter(token);

        if (!IsKey(expr))
        {
            if (flags.isCondition || flags.isProgramLine)
            {
                continue;
            }

            if (token[token.length() - 1] != ';')
            {
                std::cout << "Оператору " << token << " нужна \";\"\n";
                return false;
            }

            continue;
        }

        PascalOperator oper;
        oper.line = linePos;
        oper.key = expr;

        flags.isProgramLine = false;

        if (oper.key == "IF")
        {
            flags.isCondition = true;
        }

        if (oper.key == "UNTIL")
        {
            flags.isUntilCondition = true;
        }

        operatorStack.push(oper);

        if (IsExecuteKey(oper.key))
        {
            flags.isCondition = false;
            flags.isUntilCondition = false;

            if (oper.key == "END.")
            {
                if (!flags.foundFinalEnd)
                {
                    flags.foundFinalEnd = true;
                }
                else
                {
                    std::cout << "Найден еще 1 оператор END.\n";
                    return false;
                }
            }

            try
            {
                ExecuteOperators(operatorStack);
            }
            catch (std::runtime_error e)
            {
                std::cout << e.what();
                return false;
            }
        }
    }
    return true;
}

bool ParsePascalFile(std::istream& input)
{
    bool IsCondition = false;
    size_t linesRead = 0;
    std::string line;

    MyStack<PascalOperator> operatorStack;
    ParseFlags flags;

    while (std::getline(input, line))
    {
        linesRead++;
        line = RemoveSpacesBeforeSemicolons(line);
        if (!WorkWithOperators(linesRead, line, operatorStack, flags))
        {
            return false;
        }
    }

    return operatorStack.isEmpty();
}

int main() 
{
    setlocale(LC_ALL, "RU");

    const int keysSize = sizeof(keys) / sizeof(keys[0]);

    std::ifstream input;
    input.open("input.txt");

    if (!input.is_open())
    {
        std::cout << "Can`t open file!";
        return EXIT_FAILURE;
    }

    if (ParsePascalFile(input))
    {
        std::cout << "All good!\n";
    }

    return EXIT_SUCCESS;
}