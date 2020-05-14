#include <iostream>
#include "FixedString.h"


int main()
{
	CFixedString<64> fixedString("Hello Worlld!!!"); //fixedString(999, StrBaseEnum::BIN);

	std::cout << fixedString.data() << std::endl;

	std::cout << "No of \"l\" Occurances: " << fixedString.countNoOccurances('l') << std::endl;
	std::cout << "No of \"lo\" Occurances: " << fixedString.countNoOccurances("lo") << std::endl;


	std::cout << "Occ: " << fixedString.findLastOccurance("ll") << std::endl;


	std::cout << "SubStr: " << fixedString.substr(6, 7).data() << std::endl;


	fixedString.append("test");

	fixedString.appendPrefix("xxx");


	std::cout << fixedString.data() << std::endl;


	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cin.get();
}

