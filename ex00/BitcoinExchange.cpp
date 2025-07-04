#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange() {}
BitcoinExchange::BitcoinExchange(std::string file)
{
	std::ifstream infile(file.c_str());

	if (!infile)
		throw std::runtime_error("Error: could not open file.");

	std::string date;
	std::string num;

	std::getline(infile, num);

	while (std::getline(infile, date, ',') && std::getline(infile, num))
		_map.insert(std::make_pair(date, std::strtof(num.c_str(), NULL)));
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &copy) : _map(copy._map) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &model)
{
	if (this != &model)
		this->_map = model._map;
	return (*this);
}

BitcoinExchange::~BitcoinExchange() {}

static int parse_date(const std::string &dateString)
{
	const int daysInMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (dateString.size() != 10 || dateString[4] != '-' || dateString[7] != '-')
		return (std::cerr << "Error: Invalid date format" << std::endl, 0);

	int year, month, day;
	std::istringstream ss(dateString);

	ss >> year;
	ss.ignore();
	ss >> month;
	ss.ignore();
	ss >> day;

	if (ss.fail() || month < 1 || month > 12 || day < 1 || day > daysInMonth[month])
		return (std::cerr << "Error: Invalid date values" << std::endl, 0);
	return 1;
}

static int splitLine(const std::string &line, std::string &date, std::string &val)
{
	std::size_t pos = line.find(" | ");

	if (pos != std::string::npos)
	{
		date = line.substr(0, pos);
		val = line.substr(pos + 3);
	}
	else
		return (std::cerr << "Error: bad input => " << line << std::endl, 0);
	return 1;
}

static bool isValidPositiveFloat(const std::string &valueString)
{
	std::istringstream ss(valueString);
	float value;

	ss >> std::noskipws >> value;

	// Check if parsing succeeded, no extra characters remain, and the number is positive
	bool ret = !ss.fail() && ss.eof() && value >= 0 && value <= 1000;
	if (!ret)
		std::cerr << "Error: Invalid float value" << std::endl;
	return ret;
}

void BitcoinExchange::printValue(std::ifstream &file)
{
	std::string line, date, val;

	while (std::getline(file, line))
	{
		// Skip empty lines
		if (line.empty())
			continue;

		if (!(splitLine(line, date, val) && parse_date(date) && isValidPositiveFloat(val)))
			continue;

		// Find the first element not less than the date
		std::map<std::string, float>::iterator it = _map.lower_bound(date);

		float rate = 0.0f;

		if (it != _map.end() && it->first == date)
			rate = it->second;
		else
		{
			// No exact match, find previous date
			if (it != _map.begin())
			{
				--it; // Move to previous date (the closest lower one)
				rate = it->second;
			}
			else
				continue;
		}
		std::cout << date << " => " << val << " = " << atof(val.c_str()) * rate << std::endl;
	}
}
