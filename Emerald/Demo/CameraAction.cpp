// CameraAction Demo
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
	
	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EETexture button2Tex(L"Texture/主界面/模式标签/生涯模式.png");
	EEBox *box = new EEBox(FLOAT3(0.0f, 0.0f, 10.0f), 10.0f, 10.f, 10.f, button2Tex);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		EEShowFPSInTitle(L"- -");

		EECameraProcess(EE_CAMERA_FIRST);
		box->Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif