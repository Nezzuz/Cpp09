#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	if (argc < 3)
		return (std::cerr << "insert the armguments!" << std::endl, 1);
	PmergeMe merge;
	try
	{
		merge.sort(argc, argv);
	}
	catch (const std::exception &e) {std::cerr << e.what() << '\n';}

	return 0;
}
