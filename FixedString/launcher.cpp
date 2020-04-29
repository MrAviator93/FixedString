#include <iostream>
#include "FixedString.h"


int main()
{
	CFixedString<32> fixedString(999, StrBaseEnum::BIN);

	std::cout << fixedString.data() << std::endl;

	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cin.get();
}

