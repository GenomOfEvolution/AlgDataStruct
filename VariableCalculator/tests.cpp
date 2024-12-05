#define CATCH_CONFIG_MAIN
#include "catch2/catch_amalgamated.hpp"
#include "Calculator.h"
#include "MyStack.h"
#include <vector>

TEST_CASE("Проверить правильность скобок")
{
	setlocale(LC_ALL, "RU");
	Calculator calculator;

	calculator.variables['a'] = 1;
	calculator.variables['b'] = 2;
	calculator.variables['c'] = 3;

	const std::vector<std::string> testCases = {
		"a b c ^ ^",
		"a b ^ c ^",
		"a b / c /",
		"a b + c *",
		"a b * c ^",
		"a b - c /",

		"a b c - -",
		"a b c + -",
		"a b c - +",
		"a b c / *",
		"a b c * /",
		"a b c / /",
		"a b c / * SIN",
		"a ~ b ~ c - +",
		"a ~ b ~ c + -",
		"a ~ b ~ c - -"
	};

	const std::vector<std::string> testAnswers = {
		"a ^ (b ^ c) = 1",
		"(a ^ b) ^ c = 1",
		"(a / b) / c = 0.166667",
		"(a + b) * c = 9",
		"(a * b) ^ c = 8",
		"(a - b) / c = -0.333333",

		"a - (b - c) = 32",
		"a - (b + c) = 28",
		"a + b - c = 48",
		"a * b / c = 200",
		"a / (b * c) = 2",
		"a / (b / c) = 8",
		"SIN(a * b / c) = -0.873297",
		"-a + (-b) - c = -52",
		"-a - (-b + c) = -32",
		"-a - (-b - c) = -28"
	};

	for (int i = 0; i < testCases.size(); i++)
	{
		if (i == 6)
		{
			calculator.variables['a'] = 40;
			calculator.variables['b'] = 10;
			calculator.variables['c'] = 2;
		}

		std::string tempFilename = "tempfile.txt";
		std::ofstream tempFile(tempFilename);
		tempFile << testCases[i];
		tempFile.close();

		std::ifstream ifs(tempFilename);
		std::cout << "Input: " << testCases[i] << "\n";
		std::string res = calculator.Calc(ifs);
		std::string prikol = testAnswers[i];

		REQUIRE(res == prikol);
		std::remove(tempFilename.c_str());
	}
}
