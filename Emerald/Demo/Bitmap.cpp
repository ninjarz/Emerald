// Bitmap Demo
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
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);

	EEBitmap bitmap(L"Texture\\etc\\xmyrz.jpg");
	EEQuad2D quad(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight() + 200), bitmap);
	quad.SetLocalZOrder(10.0f);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		quad.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif