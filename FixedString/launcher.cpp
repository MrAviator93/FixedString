#include <iostream>
#include "FixedString.h"


int main()
{
	CFixedString32 fixedString("HELLO WORLD");

	auto result = fixedString.substr(6, 5);


	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cin.get();
}

