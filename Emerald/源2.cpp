#if 1
#include "Emerald.h"
//#include <xnamath.h>

#define WAVEWIDTH 100

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EEDesc desc;
	desc.applicationName = L"Emerald";  //窗口名称
	desc.isFullScreen = false;     //是否全屏
	desc.width = 800;            //窗口宽度
	desc.height = 450;           //窗口高度
	desc.isSSAA = true;           //是开启抗锯齿
	desc.isVsync = false;          //是否垂直同步
	EEInitialize(desc);

	EEMusic music;
	music.Open("Music/jx.mp3");
	music.SetFrequencyRatio(2.0f);
	//music.Start(0.2f);

	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EEScene *mainScene = new EEScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);

	EETexture nameTex(L"Texture\\主界面\\游戏名称.png");
	EEQuad *nameQuad = new EEQuad(Rect_Float(70, 170, 470, 270), nameTex);
	nameQuad->SetAlpha(0.0f);
	EEFade(nameQuad, 1.0f, 1.0f);
	mainScene->AddObject(nameQuad);

	while (EERun())
	{
		EEShowFPSInTitle(L"- -");
		EEBeginScene(EEColor::WHITE);

		EEProcess(mainScene);

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif