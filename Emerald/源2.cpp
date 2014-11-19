#if 1
#include "Emerald.h"

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
	//music.SetFrequencyRatio(1.0f);
	music.Start(0.0f);

	//order 1, time 0 - +∞
	EETexture bgTex(L"Texture\\主界面\\主界面背景.jpg");
	EEScene *mainScene = new EEScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	mainScene->SetLocalZOrder(10.0f);

	//order 0, time 0 - +∞
	EETexture nameTex(L"Texture\\主界面\\游戏名称.png");
	EEQuad *nameQuad = new EEQuad(Rect_Float(70, 170, 470, 270), nameTex);
	nameQuad->SetLocalZOrder(5.f);
	nameQuad->SetAlpha(0.0f);
	EEFade(nameQuad, 1.0f, 1.0f);
	mainScene->AddObject(nameQuad);

	//order 0.1, time 1 - +∞
	EETexture round1Tex(L"Texture\\主界面\\圆形图案1.png");
	EEQuad *round1Quad = new EEQuad(Rect_Float(70, 75, 370, 375), round1Tex);
	round1Quad->SetLocalZOrder(9.f);
	round1Quad->SetAlpha(0.0f);
	EEFade(round1Quad, 1.0f, 1.0f, 1.0f);
	EERotateYX(round1Quad, 16.0f, 2 * EE_2PI, 1.0f, true);
	mainScene->AddObject(round1Quad);

	//order 0.1, time 2 - +∞
	EETexture round2Tex(L"Texture\\主界面\\圆形图案2（滤色）.png");
	EEQuad *round2Quad = new EEQuad(Rect_Float(0, 5, 440, 445), round2Tex);
	round2Quad->SetLocalZOrder(9.f);
	round2Quad->SetAlpha(0.0f);
	EEFade(round2Quad, 1.0f, 1.0f, 2.0f);
	EERotateYX(round2Quad, 16.0f, -2 * EE_2PI, 2.0f, true);
	mainScene->AddObject(round2Quad);

	//order 0.1, time 3.5 - +∞
	EETexture road1Tex(L"Texture\\主界面\\线路形图案1（滤色）.png");
	EEQuad *road1Quad = new EEQuad(Rect_Float(300, 65, 500, 385), road1Tex);
	road1Quad->SetLocalZOrder(8.f);
	road1Quad->SetAlpha(0.0f);
	EEFade(road1Quad, 1.0f, 1.0f, 3.5f);
	mainScene->AddObject(road1Quad);

	//order 0.1, time 3.0 - +∞
	EETexture road2Tex(L"Texture\\主界面\\线路形图案2（滤色）.png");
	EEQuad *road2Quad = new EEQuad(Rect_Float(40, 45, 600, 405), road2Tex);
	road2Quad->SetLocalZOrder(8.f);
	road2Quad->SetAlpha(0.0f);
	EEFade(road2Quad, 1.0f, 1.0f, 3.0f);
	mainScene->AddObject(road2Quad);

	//order 0.1, time 3.5 - +∞
	EETexture TopTex(L"Texture\\主界面\\上框.png");
	EEQuad *TopQuad = new EEQuad(Rect_Float(0, 0, (float)EEGetWidth(), 25.f), TopTex);
	TopQuad->SetLocalZOrder(7.f);
	TopQuad->SetAlpha(0.0f);
	EEFade(TopQuad, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(TopQuad);

	//order 0.1, time 3.5 - +∞
	EETexture BottomTex(L"Texture\\主界面\\下框.png");
	EEQuad *BottomQuad = new EEQuad(Rect_Float(0, (float)EEGetHeight() - 25.f, (float)EEGetWidth(), (float)EEGetHeight()), BottomTex);
	BottomQuad->SetLocalZOrder(7.f);
	BottomQuad->SetAlpha(0.0f);
	EEFade(BottomQuad, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(BottomQuad);

	EETexture button1Tex(L"Texture/主界面/模式标签/自由模式.png");
	EEButton *button1 = new EEButton(EE_BUTTON_SCALE, Rect_Float(40.f, 380.f, 100.f, 440.f), 1.3f, 0.2f, 0.2f, button1Tex, NULL);
	button1->SetLocalZOrder(5.f);
	button1->SetAlpha(0.0f);
	EEFade(button1, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(button1);

	EETexture button2Tex(L"Texture/主界面/模式标签/生涯模式.png");
	EEButton *button2 = new EEButton(EE_BUTTON_SCALE, Rect_Float(120.f, 380.f, 180.f, 440.f), 1.3f, 0.2f, 0.2f, button2Tex, NULL);
	button2->SetLocalZOrder(5.f);
	button2->SetAlpha(0.0f);
	EEFade(button2, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(button2);

	EETexture button3Tex(L"Texture/主界面/模式标签/游戏商店.png");
	EEButton *button3 = new EEButton(EE_BUTTON_SCALE, Rect_Float(200.f, 380.f, 260.f, 440.f), 1.3f, 0.2f, 0.2f, button3Tex, NULL);
	button3->SetLocalZOrder(5.f);
	button3->SetAlpha(0.0f);
	EEFade(button3, 1.0f, 1.0f, 3.5);
	mainScene->AddObject(button3);

	EETexture musicFrameTex(L"Texture/主界面/播放器/时间轴.png");
	EETexture musicProgressTex(L"Texture/主界面/播放器/进度.png");
	EEProgressbar *progressbar = new EEProgressbar(Rect_Float(580.f, 335.f, 770.f, 340.f), Rect_Float(0.0f, 0.0f, 190.f, 5.f), musicProgressTex, musicFrameTex, NULL);
	mainScene->AddObject(progressbar);


	while (EERun())
	{
		EEBeginScene(EEColor::WHITE);

		progressbar->SetProgress(music.GetProgress());
		EEProcess(mainScene);

		EEEndScene();
	}

	EEShutdown();
	return 0;
}
#endif