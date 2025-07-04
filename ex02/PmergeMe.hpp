#pragma once
# include <iostream>
# include <list>
# include <vector>
# include <cmath>
# include <ctime>
#include <climits>

class PmergeMe
{
	private:
		std::list<std::list<int> > elements;
		std::vector<std::vector<int> > vec_elements;
		int	level;
		static const unsigned int besugo[];

	public:
		PmergeMe();
		PmergeMe(int level);
		PmergeMe(const PmergeMe& other);
		PmergeMe&	operator = (const PmergeMe& other);
		~PmergeMe();

		void		sort(int argc, char **argv);
		void		addElem(std::list<int> n);
		void		addElem(std::vector<int> n);
		void		merge();
		void		vec_merge();
		void		insert();
		void		vec_insert();
		void		insertElem(std::list<std::list<int> > &pend, std::list<std::list<int> >::iterator &pend_it);
		void		insertElem(std::vector<std::vector<int> > &pend, std::vector<std::vector<int> >::iterator &pend_it);
		void		copyValues(PmergeMe &tmp);
		void		vec_copyValues(PmergeMe &tmp);
};
void	printElemets(std::list<std::list<int> > elements);
void	printElemets(std::vector<std::vector<int> > elements);
