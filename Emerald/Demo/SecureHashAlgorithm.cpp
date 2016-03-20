// SHA Demo
#if 1
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	std::cout << EESecureHashAlgorithm::SHA_1("Hello world");

	return 0;
}

#endif