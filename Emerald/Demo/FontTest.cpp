// Font Demo
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

	EEBitmap bitmap1 = EEGetFontBitmap(L'啊');
	EEBitmap bitmap2 = EEGetFontBitmap(L'呀');
	EEBitmap bitmap3 = EEGetFontBitmap(L'？');
	EEBitmap bitmap = EEBitmapCombineHorizontal(bitmap1, bitmap2);
	//bitmap = EEBitmapCombineHorizontal(bitmap, bitmap3);
	//bitmap.Resize(500, 500);
	//std::vector<EEBitmap> bitmaps;
	//EEBitmapDivideVertical(bitmap, 3, bitmaps);
	//EETexture fonttex(bitmaps[1]);
	EEQuad2D quad0(Rect_Float(0, 0, (float)bitmap.GetWidth(), (float)bitmap.GetHeight()), bitmap);
	quad0.SetLocalZOrder(10.0f);

	EEBitmap bitmap11 = EEGetFontBitmap(L"大家好!呀\n呀\'呀呀?");
	EEQuad2D quad1(Rect_Float(0, 200, (float)bitmap11.GetWidth(), (float)bitmap11.GetHeight() + 200), bitmap11);
	quad1.SetLocalZOrder(10.0f);

	EETexture bgTex(L"Texture\\Project Diva Freedom\\01\\选歌条.png");
	EEFont helloworld(FLOAT3(100.0f, 100.0f, 0.0f), EEColor::YELLOW, L"hello world");
	helloworld.AddText(L'1');
	EELineEditor *lineEditor = new EELineEditor(Rect_Float(200.f, 300.f, 300.f, 350.f), bgTex, EEColor::BLACK);
	EELineEditor *lineEditor2 = new EELineEditor(Rect_Float(400.f, 300.f, 500.f, 350.f), bgTex, EEColor::BLACK);
	EETextEditor *textEditor = new EETextEditor(Rect_Float(300.f, 100.f, 400.f, 150.f), bgTex, EEColor::BLACK);

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		EEShowFPSInTitle(L"- -");

		quad0.Process();
		quad1.Process();
		lineEditor->Process();
		lineEditor2->Process();
		helloworld.Process();
		textEditor->Process();

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif