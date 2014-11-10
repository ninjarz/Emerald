#include "Emerald.h"

#define debugmode

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	EEInitialize();

	EEMusic music;
	music.Open("Music/jx.mp3");
	music.SetFrequencyRatio(2.0f);
	music.Start(0.2f);

	EETexture upTex(L"Texture/界面/主界面/CG鉴赏.png");
	EETexture overTex(L"Texture/界面/主界面/更多.png");
	EETexture downTex(L"Texture/界面/主界面/成就.png");
	SaveTextureToFile(EE_BMP, L"test.bmp", downTex);

	EEButton *button1 = new EEButton(EE_BUTTON_THREE, Rect_Float(300.f, 100.f, 400.f, 200.f), upTex, overTex, downTex, NULL);
	EEButton *button2 = new EEButton(EE_BUTTON_SCALE, Rect_Float(200.f, 200.f, 300.f, 300.f), 1.3f, 0.2f, 0.2f, upTex, NULL);
	EEProgressbar *progressbar = new EEProgressbar(Rect_Float(400.f, 200.f, 700.f, 250.f), Rect_Float(410.f, 210.f, 690.f, 240.f), upTex, overTex, NULL);
	progressbar->SetProgress(0.5f);
	progressbar->SetScale(2.0f);

	EETexture sceneTex(L"Texture/界面/主界面/主界面-1.png");

	EEScene *scene = new EEScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), sceneTex);
	scene->AddButton(button1);
	scene->AddButton(button2);

	EEMoveBy(button2, 5.f, FLOAT2(100.f, 100.f));
	EEScale(button2, 2.f, 2.0f);
	EEScale(button1, 2.f, 1.0f);
	EERotateBy(button1, 100.f, EE_2PI * 80, FLOAT2(400.f, 200.f));

	while (EERun())
	{
		EEShowFPSInTitle(L"- -");
		EEBeginScene(EEColor::WHITE);

		float tmp = music.GetProgress();
#ifdef debugmode
		if (tmp != 0.0f)
		{
			std::cout << tmp << std::endl;
		}
#endif
		progressbar->SetProgress(tmp);
		EEProcess(scene);
		EEProcess(progressbar);

		EEEndScene();
	}

	EEShutdown();
	return 0;
}