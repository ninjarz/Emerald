// ListView Demo
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
	desc.isVsync = true;				//是否垂直同步
	EEInitialize(desc);

	EETexture bgTex(L"Texture\\etc\\xmyrz.jpg");
	EEQuad2D quad(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	EEQuad2D quad2(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	EEListView listView(Rect_Float(0, 0, (float)EEGetWidth() / 2, (float)EEGetHeight() / 2));
	listView.AddObject(&quad);
	listView.AddObject(&quad2);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		listView.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif