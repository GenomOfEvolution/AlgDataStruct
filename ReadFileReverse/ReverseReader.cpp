/*
  27. Задан  текстовый  файл. Каждая строка содержит не более 
255 символов. Создать  новый  файл,  в  котором  строки  будут
следовать в обратном порядке. Размер файла  не ограничивается.
Запрещается размещать файл целиком в  основной  памяти.  Файлы
размером порядка 10 Мгб должны обрабатываться не  более 2 сек.
(9).

Author: Олег Нечаев ПС-24
Среда выполнения: MS VisualStudio2022 Community Edition
Источники: 
*/

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
	// TODO: Буфферизировать ввод, обрабатывать данный кусок и записывать в output
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
