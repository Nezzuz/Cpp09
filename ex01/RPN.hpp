#pragma once
#include <iostream>
#include <stack>

class RPN
{
	private:
		std::stack<int> _stack;
		
	public:
		RPN();
		RPN(const RPN& other);
		~RPN();

		void add(char input);

		int getRes();

		RPN&	operator = (const RPN& other);
};
