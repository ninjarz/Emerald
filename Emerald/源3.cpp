#if 1
#include "Emerald.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";  //窗口名称
	desc.isFullScreen = false;     //是否全屏
	desc.width = 800;            //窗口宽度
	desc.height = 450;           //窗口高度
	desc.isSSAA = true;           //是开启抗锯齿
	desc.isVsync = false;          //是否垂直同步
	EEInitialize(desc);

	EEFont helloworld(FLOAT3(100.0f, 100.0f, 0.0f), "hello world", EEColor::YELLOW);
	EELineEditer *lineEditer = new EELineEditer(Rect_Float(200.f, 300.f, 300.f, 350.f), EEColor::WHITE, EEColor::BLACK);
	EELineEditer *lineEditer2 = new EELineEditer(Rect_Float(400.f, 300.f, 500.f, 350.f), EEColor::WHITE, EEColor::BLACK);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		lineEditer->Process();
		lineEditer2->Process();
		helloworld.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif