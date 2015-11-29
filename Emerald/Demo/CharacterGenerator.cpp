// Demo
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
	EEInitialize();

	EEBitmap src(L"Texture\\etc\\xmyrz.jpg");
	EECharacterGenerator generator;

	std::string resultString;
	generator.Generate(src, resultString);

	EESetFontHeight(2);
	EEBitmap result = EEGetFontBitmap(resultString.data());

	std::ofstream fout("test.txt");
	fout << resultString;
	fout.close();

	EEQuad2D quad(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), result);
	quad.SetIsUseColor(true);
	quad.SetColor(EEColor::BLACK);

	while (EERun())
	{
		EEBeginScene(EEColor::WHITE);

		quad.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}

#endif