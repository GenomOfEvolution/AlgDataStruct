#include "ReverseReader.h";

std::optional<Args> ParseArgs(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "Wrong parametrs!\n" <<
			"Please use: ReverseReader.exe <input file> <output file>";
		return std::nullopt;
	}

	Args result;

	result.inputFileName = argv[1];
	result.outputFileName = argv[2];

	return result;
}

void ReadFileReverse(std::istream& input, std::ostream& output)
{
	std::string line;
	std::stack<int> breaksPos;

	breaksPos.push(0);
	while (std::getline(input, line))
	{
		breaksPos.push(input.tellg());
	}
	breaksPos.pop();

	while (!breaksPos.empty())
	{
		input.clear();
		input.seekg(breaksPos.top());
		breaksPos.pop();
		std::getline(input, line);
	
		output << line << std::endl;
	}
}

void ReadReverse(const Args& args)
{
	std::ifstream inputStream;
	inputStream.open(args.inputFileName);
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.inputFileName);
	}

	std::ofstream outputStream;
	outputStream.open(args.outputFileName);
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.outputFileName);
	}

	ReadFileReverse(inputStream, outputStream);

	if (!outputStream.flush())
	{
		throw std::runtime_error("Failed to save data");
	}
}