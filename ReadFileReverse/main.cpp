#include "ReverseReader.h";
#include <chrono>
using namespace std::chrono;

int main(int argc, char* argv[]) 
{
	auto start = high_resolution_clock::now();
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
	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);
	std::cout << "Elapsed time: " << static_cast<double>(duration.count()) / 1000000 << std::endl;

	return EXIT_SUCCESS;
}