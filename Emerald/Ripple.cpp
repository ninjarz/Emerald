//Ripple Demo
#if 1
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

	//EETexture bgTex(L"Texture\\etc\\xmyrz.jpg");
	EETexture bgTex(L"Texture\\test3.gif");
	
	//EETexture bgTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\背景.jpg");
	EEScene mainScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	mainScene.SetTexIndex(1);
	EERippleC ripple(bgTex);
	//ripple.Disturb((int)(100.f / 800.f * bgTex.GetWidth()), (int)(100.f / 450.f * bgTex.GetHeight()), 10, 256);
	
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		
		if (EECore::s_EECore->IsKeyDown(VK_LBUTTON))
		{
			Point pos = EEGetMousePosition();
			ripple.Disturb((int)(pos.x / 800.f * bgTex.GetWidth()), (int)(pos.y / 450.f * bgTex.GetHeight()), 10, 256);
		}
		ripple.Process();
		
		mainScene.Process();

		printf("%d ", EEGetFPS());
		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif