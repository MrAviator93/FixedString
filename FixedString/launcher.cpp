#include <iostream>
#include "FixedString.h"


int main()
{
	CFixedString<32> fixedString("Hello World"); //fixedString(999, StrBaseEnum::BIN);

	std::cout << fixedString.data() << std::endl;

	std::cout << "No of \"l\" Occurances: " << fixedString.countNoOccurances('l') << std::endl;
	std::cout << "No of \"lo\" Occurances: " << fixedString.countNoOccurances("lo") << std::endl;


	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cin.get();
}

