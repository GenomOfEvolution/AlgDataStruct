#include "ReverseReader.h"

/*
   27. Задан  текстовый  файл. Каждая строка содержит не более 
255 символов. Создать  новый  файл,  в  котором  строки  будут
следовать в обратном порядке. Размер файла  не ограничивается.
Запрещается размещать файл целиком в  основной  памяти.  Файлы
размером порядка 10 Мгб должны обрабатываться не  более 2 сек.
(9).
*/

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
	std::stack<int> breaksPos;
	int BUFFER_SIZE = 1024 * 1024;
	int buffersRead = 0;
	std::string line;

	breaksPos.push(0);
	while (input)
	{
		std::vector<char> buffer(BUFFER_SIZE);
		input.read(buffer.data(), buffer.size());
		int i = 0;
		for (i = 0; i < buffer.size(); i++)
		{
			if (buffer[i] == '\n')
			{
				i++;
				breaksPos.push(i + buffersRead * BUFFER_SIZE);
			}
		}
		buffersRead++;
	}
	breaksPos.pop();

	while (!breaksPos.empty())
	{
		input.clear();
		input.seekg(breaksPos.top());
		breaksPos.pop();
		std::getline(input, line);
	
		output << line << '\n';
	}
}

void ReadReverse(const Args& args)
{
	std::ifstream inputStream;
	inputStream.open(args.inputFileName, std::ios::binary);
	if (!inputStream.is_open())
	{
		throw std::runtime_error("Failed to open " + args.inputFileName);
	}

	std::ofstream outputStream;
	outputStream.open(args.outputFileName, std::ios::binary);
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
