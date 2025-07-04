#pragma once
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <fstream>
#include <ctime>
#include <limits>

class BitcoinExchange
{
	private:
		BitcoinExchange();
		std::map<std::string, float> _map;
	public:
		BitcoinExchange(std::string file);
		BitcoinExchange(const BitcoinExchange &copy);
		BitcoinExchange &operator= (const BitcoinExchange &model);
		~BitcoinExchange();

		void printValue(std::ifstream &file);
};
