#include "ReverseReader.h"
#include <chrono>
using namespace std::chrono;

int main(int argc, char* argv[]) 
{
	unsigned int start_time = clock();

	std::optional<Args> args = ParseArgs(argc, argv);
	if (!args)
	{
		return EXIT_FAILURE;
	}

	try 
	{
		ReadReverse(*args);
	}
	catch (const std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	unsigned int end_time = clock();

	std::cout << "Elapsed time: " << end_time - start_time << std::endl;

	return EXIT_SUCCESS;
}