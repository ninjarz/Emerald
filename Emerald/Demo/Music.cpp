// Music Demo
#if 0
#include "Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEMusic music;
	//music.AsyncLoadMusic("Music/zhntn.mp3");
	//music.Start();

	music.Open("Music/zhntn.mp3");
	music.Play(0.5, 0.1, 2);

	system("pause");
	return 0;
}
#endif