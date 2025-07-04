#include "RPN.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
		return (std::cerr << "Error: Invalid arguments" << std::endl, 1);

	RPN rpn;

	try
	{
		for (int i = 0; argv[1][i]; i++)
			rpn.add(argv[1][i]);
		std::cout << rpn.getRes() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
	return 0;
}
