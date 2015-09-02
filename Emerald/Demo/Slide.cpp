// Slide Demo
#if 0
#include "Emerald.h"


//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	EETexture bgTex(L"Texture\\test.gif");
	EESlide mainScene(Rect_Float(0, 0, (float)bgTex.GetWidth(), (float)bgTex.GetHeight()), bgTex, 0.1f);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		mainScene.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif