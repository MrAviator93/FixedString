#include <iostream>
#include "FixedString.h"

class CSomeClass
{
public:


private:
	CFixedString16 m_someString;
};

int main()
{
    std::cout << "Hello World!\n";

	CFixedString32 fixedString("HELLO WORLD");

	for (const auto& c : fixedString)
	{
		std::cout << c << std::endl;
	}

	std::cout << sizeof(CSomeClass) << std::endl;


	std::cin.get();
}

