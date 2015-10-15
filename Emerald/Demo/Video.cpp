// Video Demo
#if 1
#include "../Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";
	desc.isFullScreen = false;
	desc.width = 800;
	desc.height = 450;
	desc.isSSAA = true;
	desc.isVsync = false;
	EEInitialize(desc);

	EEVideo video;
	if (!video.Open("Demo/Video/qsx.flv"))
	{
		return 0;
	}
	// video.Play();

	auto data = video.GetData();
	int i = 0;
	EEQuad2D quad2D(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), data[0]);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		quad2D.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif