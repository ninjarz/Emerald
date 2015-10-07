// Video Demo
#if 1
#include "../Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEVideo video;

	video.Open("");
	video.Play();

	system("pause");
	return 0;
}

#endif