// Demo
#if 0
#include "../Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EENeuralNetworks networks;
	networks.Generate(2, 1, {3, 5});
	std::vector<float> result = networks.Stimulate({ 0.5, 0.5 });

	return 0;
}

#endif