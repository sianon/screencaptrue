#include "crypt_key_utils/CryptKeyUtils.h"
#include <iostream>

#pragma comment(lib, "CryptKeyUtils.lib")

int main(int argc, char* argv[])
{
	std::cout << "\n----- GetSystemID Begin -----\n" << std::endl;
	char code[64] = {0};
	::GetSysID(code);
	std::cout << "SystemID: " << code << std::endl;
	std::cout << "\n----- GetSystemID End -----\n" << std::endl;
	std::getchar();
	return 0;
}

