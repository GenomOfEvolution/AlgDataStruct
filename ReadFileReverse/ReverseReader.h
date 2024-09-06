#include <string>
#include <fstream>
#include <optional>
#include <iostream>
#include <stack>

struct Args
{
	std::string inputFileName;
	std::string outputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[]);
void ReadReverse(const Args& args);
