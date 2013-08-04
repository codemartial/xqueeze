#include <iostream>
#include <xqml/xqagenerator.hpp>

int main()
{
	xqML::xqAGenerator gen(std::cin, std::cout);
	gen.generate();
	return 0;
}
