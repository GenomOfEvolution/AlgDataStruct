#pragma once
#include <string>
#include <optional>
#include <iostream>

struct Args
{
	std::string inputFileName;
};

std::optional<Args> ParseArgs(int argc, char* argv[]);