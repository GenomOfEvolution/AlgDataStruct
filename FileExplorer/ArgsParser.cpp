#include "ArgsParser.h"

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Wrong parametrs!\n" <<
			"Please use: FileExplorer.exe <input file>";
		return std::nullopt;
	}

	Args result;

	result.inputFileName = argv[1];

	return result;
}
