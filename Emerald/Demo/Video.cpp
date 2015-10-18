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


	EEVideo video;
	if (!video.Open("Demo/Video/qsx.flv"))
	{
		return 0;
	}
	// video.Play();


	int i = 0;
	EEQuad2D quad2D(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), video.GetFrame(0));

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		i = ++i % video.GetPacketSize();
		quad2D.SetTexture(video.GetFrame(i));
		quad2D.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif