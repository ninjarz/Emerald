//Font Demo
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

	EEBitmap bitmap1 = EEGetFontBitmap(L'啊');
	EEBitmap bitmap2 = EEGetFontBitmap(L'呀');
	EEBitmap bitmap3 = EEGetFontBitmap(L'？');
	EEBitmap bitmap = EEBitmapCombineHorizontal(bitmap1, bitmap2);
	bitmap = EEBitmapCombineHorizontal(bitmap, bitmap3);
	//bitmap.Resize(500, 500);
	//std::vector<EEBitmap> bitmaps;
	//EEBitmapDivideVertical(bitmap, 3, bitmaps);
	//EETexture fonttex(bitmaps[1]);
	EEQuad2D quad0(Rect_Float(0, 0, (float)bitmap.GetWidth(), (float)bitmap.GetHeight()), bitmap);
	quad0.SetLocalZOrder(10.0f);

	EEBitmap bitmap11 = EEGetFontBitmap(L"大家好!呀呀\'呀呀?");
	EEQuad2D quad1(Rect_Float(0, 200, (float)bitmap11.GetWidth(), (float)bitmap11.GetHeight() + 200), bitmap11);
	quad1.SetLocalZOrder(10.0f);

	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EETexture button2Tex(L"Texture/主界面/模式标签/生涯模式.png");
	EEFont helloworld(FLOAT3(100.0f, 100.0f, 0.0f), EEColor::YELLOW, L"hello world");
	helloworld.AddText(L'1');
	EELineEditer *lineEditer = new EELineEditer(Rect_Float(200.f, 300.f, 300.f, 350.f), bgTex, EEColor::BLACK);
	EELineEditer *lineEditer2 = new EELineEditer(Rect_Float(400.f, 300.f, 500.f, 350.f), bgTex, EEColor::BLACK);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		EEShowFPSInTitle(L"- -");

		quad0.Process();
		quad1.Process();
		lineEditer->Process();
		lineEditer2->Process();
		helloworld.Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif