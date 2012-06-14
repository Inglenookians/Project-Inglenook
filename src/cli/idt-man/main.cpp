#include <iostream>

#include <ild/ILD.h>

int main(int argc, char* argv[])
{
	inglenook::log::ILD abc;
	std::cout << abc.getNumber() << std::endl;
}
