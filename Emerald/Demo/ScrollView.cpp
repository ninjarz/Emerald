// ScrollView Demo
#if 1
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
	EEScrollView scrollView(Rect_Float(0, 0, (float)EEGetWidth() / 2, (float)EEGetHeight() / 2));
	scrollView.SetContent(&quad);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		scrollView.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif