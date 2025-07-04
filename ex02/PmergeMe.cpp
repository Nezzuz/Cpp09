#include "PmergeMe.hpp"

PmergeMe::PmergeMe() { level = 0; }
PmergeMe::PmergeMe(int level) { this->level = level; }
PmergeMe::PmergeMe(const PmergeMe &other) : elements(other.elements), level(other.level) {}
PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	elements = other.elements;
	level = other.level;
	return *this;
}
PmergeMe::~PmergeMe() {}

const unsigned int PmergeMe::besugo[] = {3, 5, 11, 21, 43, 85, 171, 341,
										 683, 1365, 2731, 5461, 10923, 21845, 43691, 87381, 174763, 349525};

bool isNum(const std::string &s)
{
	if (s.empty())
		return false;
	for (size_t i = 0; i < s.size(); i++)
		if (!isdigit(s[i]))
			return false;
	return true;
}

void PmergeMe::addElem(std::list<int> n)
{
	unsigned int pairSize = (int)pow(2, level);
	if (pairSize == 0)
		pairSize = 1;

	if (elements.empty() || elements.back().size() >= pairSize)
	{
		elements.push_back(n);
		return;
	}
	if (elements.back().back() < n.back())
		elements.back().splice(elements.back().end(), n);
	else
		elements.back().splice(elements.back().begin(), n);

}

void	PmergeMe::addElem(std::vector<int> n)
{
	unsigned int pairSize = (int)pow(2, level);
	if (pairSize == 0)
		pairSize = 1;

	if (vec_elements.empty() || vec_elements.back().size() >= pairSize)
	{
		vec_elements.push_back(n);
		return;
	}
	if (vec_elements.back().back() < n.back())
		vec_elements.back().insert(vec_elements.back().end(), n.begin(), n.end());
	else
		vec_elements.back().insert(vec_elements.back().begin(), n.begin(), n.end());
}

void	PmergeMe::copyValues(PmergeMe &tmp)
{
	// Flatten tmp.elements into a single list
	std::list<int> tmp_flat;
	for (std::list<std::list<int> >::iterator it = tmp.elements.begin(); it != tmp.elements.end(); ++it)
		tmp_flat.splice(tmp_flat.end(), *it); // Move nodes instead of copying

	// Rebuild original structure with new order
	std::list<int>::iterator flat_it = tmp_flat.begin();
	for (std::list<std::list<int> >::iterator outer_it = elements.begin();
		 outer_it != elements.end() && flat_it != tmp_flat.end();
		 ++outer_it)
	{
		std::list<int> new_group;
		for (size_t i = 0; i < outer_it->size() && flat_it != tmp_flat.end(); ++i)
			new_group.splice(new_group.end(), tmp_flat, flat_it++);
		outer_it->swap(new_group); // Replace the old group with the new one
	}
}

void	PmergeMe::vec_copyValues(PmergeMe &tmp)
{
	// Flatten tmp.elements into a single list
	std::vector<int> tmp_flat;
	for (std::vector<std::vector<int> >::iterator it = tmp.vec_elements.begin(); it != tmp.vec_elements.end(); ++it)
		tmp_flat.insert(tmp_flat.end(), it->begin(), it->end()); // Move nodes instead of copying

	// Rebuild original structure with new order
	std::vector<int>::iterator flat_it = tmp_flat.begin();
	for (std::vector<std::vector<int> >::iterator outer_it = vec_elements.begin();
		 outer_it != vec_elements.end() && flat_it != tmp_flat.end();
		 ++outer_it)
	{
		std::vector<int> new_group;
		for (size_t i = 0; i < outer_it->size() && flat_it != tmp_flat.end(); ++i, ++flat_it)
			new_group.push_back(*flat_it);
		*outer_it = new_group;
	}
}

void	PmergeMe::insertElem(std::list<std::list<int> > &pend, std::list<std::list<int> >::iterator &pend_it)
{
	int key = pend_it->back(); // Use the last value as the key for ordering
	//std::cout << pend.size() << std::endl;
	// Find the correct position in elements to insert this group
	std::list<std::list<int> >::iterator insert_pos = elements.begin();
	while (insert_pos != elements.end() && insert_pos->back() < key)
		++insert_pos;

	// Splice the group into the right spot in elements
	elements.splice(insert_pos, pend, pend_it);
}

void	PmergeMe::insertElem(std::vector<std::vector<int> > &pend, std::vector<std::vector<int> >::iterator &pend_it)
{
	int key = pend_it->back(); // Use the last value as the key for ordering

	// Find the correct position in elements to insert this group
	std::vector<std::vector<int> >::iterator insert_pos = vec_elements.begin();
	while (insert_pos != vec_elements.end() && insert_pos->back() < key)
		++insert_pos;

	vec_elements.insert(insert_pos, *pend_it);
	// Erase the original from pend
	pend.erase(pend_it);
	// Reset pend_it to the beginning for the caller (optional or caller handles it)
	pend_it = pend.begin();
}

void PmergeMe::insert()
{
	unsigned int pairSize = (int)pow(2, level);
	if (pairSize == 0)
		pairSize = 1;
		
	std::list<std::list<int> > pend;
	std::list<std::list<int> >::iterator it = elements.begin();
	std::advance(it, 2);
	for (int i = 1; it != elements.end(); i++)
	{
		// Save next before splice, since 'it' will be invalidated
		std::list<std::list<int> >::iterator toMove = it++;
		if (toMove->size() == pairSize && i%2)
			pend.splice(pend.end(), elements, toMove); // Move list from elements to pend
	}
	size_t besugo_len = sizeof(besugo) / sizeof(unsigned int);
	for (size_t i = 1; i < besugo_len && !pend.empty(); ++i)
	{
		unsigned int count = besugo[i] - besugo[i - 1];

		if (count >= pend.size())
			count = pend.size(); // Clamp to size
		if (count == 0)
			continue;
		std::list<std::list<std::list<int> >::iterator> targets;

		it = pend.begin();
		for (unsigned int j = 0; j < count && it != pend.end(); ++j, ++it)
			targets.push_back(it);
		
		// Now insert in reverse order using only the targets list
		std::list<std::list<std::list<int> >::iterator>::reverse_iterator rit;
		while (!targets.empty())
		{
			rit = targets.rbegin();
			insertElem(pend, *rit);
			targets.pop_back(); // removes from the back safely
		}
	}
}

void PmergeMe::vec_insert()
{
	unsigned int pairSize = (int)pow(2, level);
	if (pairSize == 0)
		pairSize = 1;

	std::vector<std::vector<int> > pend;
	for (size_t i = 2; i < vec_elements.size(); )
	{
		if (vec_elements[i].size() == pairSize && (i % 2 == 1))
		{
			pend.push_back(vec_elements[i]);
			vec_elements.erase(vec_elements.begin() + i);
		}
		else
			++i;
	}
	size_t besugo_len = sizeof(besugo) / sizeof(unsigned int);
	for (size_t i = 1; i < besugo_len && !pend.empty(); ++i)
	{
		unsigned int count = besugo[i] - besugo[i - 1];

		if (count >= pend.size())
			count = pend.size(); // Clamp to size
		if (count == 0)
			continue;

		// Process in reverse order using index (no invalidation)
		for (int j = count - 1; j >= 0; --j)
		{
			std::vector<std::vector<int> >::iterator pend_it = pend.begin() + j;
			insertElem(pend, pend_it);
		}
	}
}

void PmergeMe::merge()
{
	PmergeMe tmp(level + 1);
	unsigned int pairSize = (int)pow(2, level);
	std::list<std::list<int> >::iterator it;

	for (it = elements.begin(); it != elements.end(); it++)
		if (it->size() == pairSize)
			tmp.addElem(*it);
	if (tmp.elements.size() > 2)
		tmp.merge();
	else if (tmp.elements.size() == 2)
		if (tmp.elements.front().size() == tmp.elements.back().size())
			tmp.merge();
	this->copyValues(tmp);
	this->insert();
}

void PmergeMe::vec_merge()
{
	PmergeMe tmp(level + 1);
	unsigned int pairSize = (int)pow(2, level);
	std::vector<std::vector<int> >::iterator it;

	for (it = vec_elements.begin(); it != vec_elements.end(); it++)
		if (it->size() == pairSize)
			tmp.addElem(*it);
	if (tmp.vec_elements.size() > 2)
		tmp.vec_merge();
	else if (tmp.vec_elements.size() == 2)
		if (tmp.vec_elements.front().size() == tmp.vec_elements.back().size())
			tmp.vec_merge();

	this->vec_copyValues(tmp);
	this->vec_insert();
	//this->printElemets();
	// tmp.printElemets();
}

void PmergeMe::sort(int argc, char **argv)
{
	std::clock_t start, end;
	for (int i = 1; i < argc; i++)
	{
		if (!isNum(argv[i]))
			throw std::runtime_error("Error: Only insert positive integers!");
		else if (atof(argv[i]) > INT_MAX)
			throw std::runtime_error("Error: Don't insert numbers bigger than INT_MAX!");
		std::list<int> list;
		std::vector<int> vector;
		list.push_back(atoi(argv[i]));
		vector.push_back(atoi(argv[i]));
		this->addElem(list);
		this->addElem(vector);
	}

	std::cout << "Before:\t\t";
	printElemets(elements);
	//list sort
	start = std::clock();
	this->merge();
	level = 0;
	end = std::clock();
	double list_duration = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "After:\t\t";
	printElemets(elements);
	std::cout << "Time to procress a range of " << elements.size()
		<< " elements with std::list :\t" << list_duration << " miliseconds" << std::endl;

	//vector sort
	start = std::clock();
	this->vec_merge();
	end = std::clock();
	double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Time to procress a range of " << elements.size() <<
		" elements with std::vector :\t" << duration << " miliseconds" << std::endl;
}

void printElemets(std::list<std::list<int> > elements)
{
	if (elements.empty())
	{
		std::cout << "[]" << std::endl;
		return;
	}
	std::list<std::list<int> >::const_iterator outer_it;
	for (outer_it = elements.begin(); outer_it != elements.end(); ++outer_it)
	{
		//std::cout << "[";
		std::list<int>::const_iterator inner_it = outer_it->begin();
		if (inner_it != outer_it->end())
		{
			std::cout << *inner_it;
			++inner_it;
			for (; inner_it != outer_it->end(); ++inner_it)
				std::cout << ", " << *inner_it;
		}
		std::cout << " ";
	}
	std::cout << std::endl;
}

void printElemets(std::vector<std::vector<int> > elements)
{
	if (elements.empty())
	{
		std::cout << "[]" << std::endl;
		return;
	}
	std::vector<std::vector<int> >::const_iterator outer_it;
	for (outer_it = elements.begin(); outer_it != elements.end(); ++outer_it)
	{
		std::cout << "[";
		std::vector<int>::const_iterator inner_it = outer_it->begin();
		if (inner_it != outer_it->end())
		{
			std::cout << *inner_it;
			++inner_it;
			for (; inner_it != outer_it->end(); ++inner_it)
				std::cout << ", " << *inner_it;
		}
		std::cout << "] ";
	}
	std::cout << std::endl;
}
