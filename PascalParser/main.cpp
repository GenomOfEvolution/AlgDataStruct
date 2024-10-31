// 26. Программа на ПАСКАЛЕ включает такие  сочетания ключевых
// слов, как    REPEAT..UNTIL, IF..THEN..ELSE, BEGIN..END,
// RECORD..END.Конец оператора  определяется  точкой  с  запятой
// (";").Требуется  проверить  правильность  вложенности данных
// конструкций с учетом допустимости взаимных вложений.В случае
// ошибок указать номер первой некорректной строки(12).

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <cctype> 

#include "MyStack.h"

const std::string keys[] = { "IF", "THEN", "ELSE", "RECORD", "END", "END.", "BEGIN", "REPEAT", "UNTIL" };

struct PascalOperator
{
    int line = 0;
    std::string key = "";
};

bool IsKey(const std::string& str)
{
    auto it = std::find(std::begin(keys), std::end(keys), str);
    return it != std::end(keys);
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


void WorkWithOperators(const size_t linePos, const std::string& str, MyStack<PascalOperator>& operatorStack)
{
    if (IsStringEmptyOrWhitespace(str))
    {
        return;
    }

    std::istringstream iss(str);
    std::string token;
    while (iss >> token)
    {
        if (token == "PROGRAM")
        {
            return;
        }

        std::string expr = CopyLettersUntilNonLetter(token);

        if (!IsKey(expr))
        {
            continue;
        }

        PascalOperator oper;
        oper.line = linePos;
        oper.key = expr;

        operatorStack.push(oper);

        std::cout << token << "\n";
    }
    std::cout << "---------\n";
}



bool ParsePascalFile(std::istream& input)
{
    bool IsCondition = false;
    size_t linesRead = 0;
    std::string line;

    MyStack<PascalOperator> operatorStack;


    while (std::getline(input, line))
    {
        linesRead++;
        line = RemoveSpacesBeforeSemicolons(line);
        WorkWithOperators(linesRead, line, operatorStack);
    }
    return true;
}


int main() 
{
    const int keysSize = sizeof(keys) / sizeof(keys[0]);

    std::map<std::string, std::map<std::string, bool>> matrix;

    // Инициализация 2-мерного массива значением true
    for (int i = 0; i < keysSize; ++i) 
    {
        for (int j = 0; j < keysSize; ++j) 
        {
            matrix[keys[i]][keys[j]] = true;
        }
    }

    // Установка что может идти перед чем
    matrix["IF"]["END."] = false;
    matrix["THEN"]["END."] = false;
    matrix["ELSE"]["END."] = false;
    matrix["END."]["END."] = false;
    matrix["REPEAT"]["END."] = false;
    matrix["RECORD"]["END."] = false;

    std::ifstream input;
    input.open("input.txt");

    if (!input.is_open())
    {
        std::cout << "Can`t open file!";
        return EXIT_FAILURE;
    }

    ParsePascalFile(input);


    return EXIT_SUCCESS;
}