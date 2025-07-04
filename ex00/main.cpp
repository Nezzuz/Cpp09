#include "BitcoinExchange.hpp"

static int	parse(int argc, std::ifstream &fin) {
	std::string		line;

	if (argc != 2)
		return (std::cerr << "Enter one file as the argument!" << std::endl, 0);
	if (!fin)
		return(std::cerr << "Error, could not open file." << std::endl, 0);
	if (std::getline(fin, line, '\n') && line != "date | value")
		return(std::cerr << "Error, infile has the wrong format" << std::endl, 0);
	return 1;
}

int main(int argc, char **argv)
{
	std::ifstream	fin(argv[1]);

	if (!parse(argc, fin))
		return 0;
	try
	{
		BitcoinExchange db("data.csv");
		db.printValue(fin);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}
