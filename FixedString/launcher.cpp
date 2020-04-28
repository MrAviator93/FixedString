#include <iostream>
#include "FixedString.h"


int main()
{
	CFixedString32 fixedString("HELLO WORLD");

	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cin.get();
}

