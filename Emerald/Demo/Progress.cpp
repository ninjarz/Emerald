//Progress Demo
#if 0
#include "../Emerald.h"

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

	EETexture musicFrameTex(L"Texture/主界面/播放器/时间轴.png");
	EETexture musicProgressTex(L"Texture/主界面/播放器/进度.png");
	//width:190 height:5
	EEProgressbar progressbar(Rect_Float(520.f, 335.f, 800.f, 340.f), Rect_Float(0.0f, 0.0f, 190.f, 5.f), musicProgressTex, musicFrameTex);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		progressbar.Process();
		
		//printf("%d ", EEGetFPS());
		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif