// Demo
#if 1
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EENeuralNetworks networks;
	networks.Generate(2, 1, {100, 200});


	return 0;
}

#endif