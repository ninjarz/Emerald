// Video Demo
#if 1
#define _EE_MEMORYLEAK_ON_
#include "../Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	// _CrtSetBreakAlloc(134);
	EEDesc desc;
	desc.applicationName = L"Emerald";
	desc.isFullScreen = false;
	desc.width = 800;
	desc.height = 450;
	desc.isSSAA = true;
	desc.isVsync = true;
	EEInitialize(desc);


	EEVideo video(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()));
	if (!video.Open("Demo/Video/qsx.flv"))
	{
		return 0;
	}
	video.Play();

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		video.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif