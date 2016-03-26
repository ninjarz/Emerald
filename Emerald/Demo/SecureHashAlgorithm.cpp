// SHA Demo
#if 1
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	std::string test("Hello world");
	std::cout << EESecureHashAlgorithm::SHA_1(test) << std::endl;
	std::cout << EESecureHashAlgorithm::SHA_2_224(test) << std::endl;
	std::cout << EESecureHashAlgorithm::SHA_2_256(test) << std::endl;
	std::cout << EESecureHashAlgorithm::SHA_2_512(test) << std::endl;

	system("pause");
	return 0;
}

#endif