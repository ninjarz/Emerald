//Game
#include "DIVAHelper.h"
#if _DIVA_
#include "Emerald.h"
#define _EE_MEMORYLEAK_ON_
#include "EEMemoryLeak.h"


int MainScene();
int SelectMusic();
int SimpleSet();
int FreePlay();

int MainScene()
{
	int flag = 0;
	//EEMusic music;
	//music.Open("Music/jx.mp3");
	//music.SetFrequencyRatio(2.0f);
	//music.Play(0.2f, 0.1f, 5);
	//music.Play(0.2f, 0.1f, 2);

	//order 10, time 0 - +∞
	EETexture bgTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\背景.jpg");
	EEScene mainScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	mainScene.SetLocalZOrder(10.0f);
	//EERotateYX(mainScene, 16.0f, 2 * EE_2PI, 1.0f, true);
	
	//order 1, time 0 - +∞
	EETexture nameTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\PDF.png");
	EEQuad2D nameQuad(Rect_Float((float)EEGetWidth() * 0.0325f, (float)EEGetHeight() * 0.3733f, (float)EEGetWidth() * 0.67375f, (float)EEGetHeight() * 0.63222f), nameTex);
	nameQuad.SetLocalZOrder(5.f);
	nameQuad.SetAlpha(0.0f);
	EEFade(&nameQuad, 1.0f, 1.0f);
	mainScene.AddObject(&nameQuad);

	//order 9, time 1 - +∞
	EETexture round1Tex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\圈1.png");
	EEQuad2D round1Quad(Rect_Float((float)EEGetWidth() * 0.083125f, (float)EEGetHeight() * 0.1456f, (float)EEGetWidth() * 0.434375f, (float)EEGetHeight() * 0.7689f), round1Tex);
	round1Quad.SetLocalZOrder(9.f);
	round1Quad.SetAlpha(0.0f);
	EEFade(&round1Quad, 1.0f, 1.0f, 1.0f);
	EERotateYX(&round1Quad, 25.0f, 1.5f * EE_2PI, 1.0f, true);
	mainScene.AddObject(&round1Quad);

	//order 9, time 2 - +∞
	EETexture round2Tex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\圈2.png");
	EEQuad2D round2Quad(Rect_Float((float)EEGetWidth() * -0.00875f, (float)EEGetHeight() * -0.018889f, (float)EEGetWidth() * 0.52625f, (float)EEGetHeight() * 0.93222f), round2Tex);
	round2Quad.SetLocalZOrder(9.f);
	round2Quad.SetAlpha(0.0f);
	EEFade(&round2Quad, 1.0f, 1.0f, 2.0f);
	EERotateYX(&round2Quad, 25.0f, -1.5f * EE_2PI, 2.0f, true);
	mainScene.AddObject(&round2Quad);

	//order 8, time 3.5 - +∞
	EETexture road1Tex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\线1.png");
	EEQuad2D road1Quad(Rect_Float((float)EEGetWidth() * 0.05875f, (float)EEGetHeight() * 0.0922f, (float)EEGetWidth() * 0.70375f, (float)EEGetHeight() * 0.8267f), road1Tex);
	road1Quad.SetLocalZOrder(8.f);
	road1Quad.SetAlpha(0.0f);
	EEFade(&road1Quad, 1.0f, 1.0f, 3.5f);
	mainScene.AddObject(&road1Quad);

	//order 8, time 3.0 - +∞
	EETexture road2Tex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\线2.png");
	EEQuad2D road2Quad(Rect_Float((float)EEGetWidth() * 0.340625f, (float)EEGetHeight() * 0.1333f, (float)EEGetWidth() * 0.658125f, (float)EEGetHeight() * 0.7456f), road2Tex);
	road2Quad.SetLocalZOrder(8.f);
	road2Quad.SetAlpha(0.0f);
	EEFade(&road2Quad, 1.0f, 1.0f, 3.5f);
	mainScene.AddObject(&road2Quad);

	//order 7, time 3.5 - +∞
	EETexture TopTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\顶.png");
	EEQuad2D TopQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0711f), TopTex);
	TopQuad.SetLocalZOrder(7.f);
	TopQuad.SetAlpha(0.0f);
	EEFade(&TopQuad, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&TopQuad);

	EETexture dpTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\dp.png");
	EEQuad2D dpQuad(Rect_Float((float)EEGetWidth() * 0.726875f, (float)EEGetHeight() * 0.0111f, (float)EEGetWidth() * 0.784375f, (float)EEGetHeight() * 0.0578f), dpTex);
	dpQuad.SetLocalZOrder(6.f);
	dpQuad.SetAlpha(0.0f);
	EEFade(&dpQuad, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&dpQuad);

	EETexture number[10] = {
		L"Texture/Project Diva Freedom\\主界面\\默认主题/0.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/1.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/2.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/3.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/4.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/5.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/6.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/7.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/8.png",
		L"Texture/Project Diva Freedom\\主界面\\默认主题/9.png" };
	EEIntBoard intBoard(Rect_Float((float)EEGetWidth() * 0.784375f, (float)EEGetHeight() * 0.0111f, (float)EEGetWidth() * 0.984375f, (float)EEGetHeight() * 0.0578f), 8, number);
	intBoard.SetLocalZOrder(6.0f);
	intBoard.SetAlpha(0.0f);
	EEFade(&intBoard, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&intBoard);

	//order 7, time 3.5 - +∞
	EETexture bottomTex(L"Texture\\Project Diva Freedom\\主界面\\默认主题\\底.png");
	EEQuad2D bottomQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.9122f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), bottomTex);
	bottomQuad.SetLocalZOrder(7.f);
	bottomQuad.SetAlpha(0.0f);
	EEFade(&bottomQuad, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&bottomQuad);

	EETexture button1Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/自由模式.png");
	EEButton button1(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.03f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.1075f, (float)EEGetHeight() * 0.9856f), 1.3f, 0.2f, 0.2f, button1Tex, [&flag](){ flag = 1; });
	button1.SetLocalZOrder(5.f);
	button1.SetAlpha(0.0f);
	EEFade(&button1, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button1);

	EETexture button2Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/生涯模式.png");
	EEButton button2(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.1375f, (float)EEGetHeight() * 0.85f, (float)EEGetWidth() * 0.21375f, (float)EEGetHeight() * 0.9844f), 1.3f, 0.2f, 0.2f, button2Tex);
	button2.SetLocalZOrder(5.f);
	button2.SetAlpha(0.0f);
	EEFade(&button2, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button2);

	EETexture button3Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/游戏商店.png");
	EEButton button3(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.24625f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.3225f, (float)EEGetHeight() * 0.9844f), 1.3f, 0.2f, 0.2f, button3Tex);
	button3.SetLocalZOrder(5.f);
	button3.SetAlpha(0.0f);
	EEFade(&button3, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button3);

	EETexture button4Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/CG鉴赏.png");
	EEButton button4(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.353125f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.42875f, (float)EEGetHeight() * 0.9844f), 1.3f, 0.2f, 0.2f, button4Tex);
	button4.SetLocalZOrder(5.f);
	button4.SetAlpha(0.0f);
	EEFade(&button4, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button4);

	EETexture button5Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/成就.png");
	EEButton button5(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.461875f, (float)EEGetHeight() * 0.85f, (float)EEGetWidth() * 0.538125f, (float)EEGetHeight() * 0.9844f), 1.3f, 0.2f, 0.2f, button5Tex);
	button5.SetLocalZOrder(5.f);
	button5.SetAlpha(0.0f);
	EEFade(&button5, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button5);

	EETexture button6Tex(L"Texture/Project Diva Freedom\\主界面\\默认主题/设置.png");
	EEButton button6(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.57f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.645625f, (float)EEGetHeight() * 0.9844f), 1.3f, 0.2f, 0.2f, button6Tex);
	button6.SetLocalZOrder(5.f);
	button6.SetAlpha(0.0f);
	EEFade(&button6, 1.0f, 1.0f, 3.5);
	mainScene.AddObject(&button6);

	EETexture musicFrameTex(L"Texture/主界面/播放器/时间轴.png");
	EETexture musicProgressTex(L"Texture/主界面/播放器/进度.png");
	//width:190 height:5
	EEProgressbar progressbar(Rect_Float(820.f, 335.f, 1100.f, 340.f), Rect_Float(0.0f, 0.0f, 190.f, 5.f), musicProgressTex, musicFrameTex);
	progressbar.SetLocalZOrder(5.f);
	mainScene.AddObject(&progressbar);
	EEQuad2D musicPos(Rect_Float(580.f, 305.f, 800.f, 370.f));
	mainScene.AddObject(&musicPos);
	musicPos.SetOverFunc(
		[&progressbar]
	{
		FLOAT3 pos = progressbar.GetPosition();
		if (pos.x > 580.0f)
		{
			pos.x -= (float)(EEGetDeltaTime() * 1000);
			if (pos.x < 580.f)
				pos.x = 580.f;
			progressbar.SetPosition(pos);
		}
	});
	musicPos.SetUpFunc(
		[&progressbar]
	{
		FLOAT3 pos = progressbar.GetPosition();
		if (pos.x < 820.0f)
		{
			pos.x += (float)(EEGetDeltaTime() * 1000);
			if (pos.x > 820.0f)
				pos.x = 820.0f;
			progressbar.SetPosition(pos);
		}
	});

	EETexture particleTex(L"Texture/主界面/随机上升的音符效果（线性减淡）.png");
	EEParticleInfo info;
	info.amount = 70;
	//life
	info.durationTime = 4.f;
	info.deltaDurationTime = 3.f;
	//position
	info.position = FLOAT3(800.f, 400.f, 0.0);
	info.deltaPosition = FLOAT3(50.f, 50.f, 0.0);
	info.width = 50;
	info.height = 50;
	info.direction = FLOAT3(-40.0, -30.0f, 0.0);
	info.deltaDirection = FLOAT3(20.0, 30.0f, 0.0);
	//color
	info.beginColor = EEColor(1.0f, 1.0f, 1.0f, 1.0f);
	info.deltaBeginColor = EEColor(0.2f, 0.2f, 0.0f, 0.0f);
	info.endColor = EEColor(0.8f, 0.8f, 1.0f, 0.0f);
	info.deltaEndColor = EEColor(0.26f, 0.26f, 0.0f, 0.0f);
	//scale
	info.beginScale = 1.0f;
	info.deltaBeginScale = 0.2f;
	info.endScale = 4.0f;
	info.deltaEndScale = 0.0f;
	//texture
	info.texture = particleTex;
	EEParticleEmitter particle(info);
	particle.SetLocalZOrder(9.0f);
	mainScene.AddObject(&particle);

	while (EERun() && flag == 0)
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		intBoard.SetValue(EEGetFPS());
		EEProcess(&mainScene);

		EEEndScene();
	}
	EETimerStop();
	round1Quad.RemoveThread();
	round2Quad.RemoveThread();

	float finishTime = 1.4f;
	float deltaTime = 0.4f;
	switch (flag)
	{
	case 1:
		EEFade(&nameQuad, deltaTime, 0.0f);
		EEFade(&road1Quad, deltaTime, 0.0f);
		EEFade(&road2Quad, deltaTime, 0.0f);
		EEFade(&particle, deltaTime, 0.0f);

		round1Quad.SetRotation(MATRIX::IDENTITY);
		round2Quad.SetRotation(MATRIX::IDENTITY);
		EEMoveBy(&round1Quad, finishTime, FLOAT2(-850.f, 0.0f), deltaTime);
		EEMoveBy(&round2Quad, finishTime, FLOAT2(-850.f, 0.0f), deltaTime);

		EEMoveBy(&TopQuad, finishTime, FLOAT2(0.f, -64.0f), deltaTime);
		EEMoveBy(&dpQuad, finishTime, FLOAT2(0.f, -64.0f), deltaTime);
		EEMoveBy(&intBoard, finishTime, FLOAT2(0.f, -64.0f), deltaTime);

		EEMoveBy(&bottomQuad, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button1, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button2, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button3, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button4, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button5, finishTime, FLOAT2(0.f, 150.0f), deltaTime);
		EEMoveBy(&button6, finishTime, FLOAT2(0.f, 150.0f), deltaTime);

		EEFade(&mainScene, deltaTime, 0.0f, 1.0f);
		EESetRuntime(finishTime);
		while (EERun())
		{
			EEBeginScene(EEColor::BLACK);

			intBoard.SetValue(EEGetFPS());
			EEProcess(&mainScene);

			EEEndScene();
		}
		break;
	}


	EERemoveThread();
	return flag;
}

int SelectMusic()
{
	int flag = 0;
	int difficult = 0;

	//scene
	EEScene selectScene(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()));
	selectScene.SetLocalZOrder(10.0f);
	selectScene.SetAlpha(0.0f);
	EEFade(&selectScene, 0.4f, 1.0f);

	//bg
	EETexture bgTex[4] = {
		L"Texture\\Project Diva Freedom\\Song Choosing\\Easy\\Easy-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Normal\\Normal-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Hard\\Hard-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Extreme\\Extreme-Background.jpg",
	};
	EESlide bgSlide(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex, 4);
	bgSlide.SetLocalZOrder(9.9f);
	selectScene.AddObject(&bgSlide);

	//circle
	EETexture circleTex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Shiny Circle（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Shiny Circle（颜色减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Shiny Circle（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Shiny Circle（颜色减淡）.png",
	};
	EESlide circleSlide(Rect_Float((float)EEGetWidth() * -0.2925f, (float)EEGetHeight() * -0.1522f, (float)EEGetWidth() * 0.395f, (float)EEGetHeight() * 1.0711f), circleTex, 4);
	circleSlide.SetLocalZOrder(9.f);
	EERotateYX(&circleSlide, 25.0f, 1.5f * EE_2PI, 1.0f, true);
	selectScene.AddObject(&circleSlide);

	//line1
	EETexture line1Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Line（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Line（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Line（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Line（滤色）.png",
	};
	EESlide line1Slide(Rect_Float((float)EEGetWidth() * 0.15f, (float)EEGetHeight() * 0.1556f, (float)EEGetWidth() * 0.509375f, (float)EEGetHeight() * 0.8444f), line1Tex, 4);
	line1Slide.SetLocalZOrder(8.f);
	selectScene.AddObject(&line1Slide);

	//line2
	EETexture line2Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Line2（滤色）.png",
	};
	EESlide line2Slide(Rect_Float((float)EEGetWidth() * -0.169375f, (float)EEGetHeight() * 0.1056f, (float)EEGetWidth() * 0.56f, (float)EEGetHeight() * 0.9233f), line2Tex, 4);
	line2Slide.SetLocalZOrder(8.f);
	selectScene.AddObject(&line2Slide);

	//star
	EETexture starTex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Star（线性减淡）.png",
	};
	EESlide starSlide(Rect_Float((float)EEGetWidth() * -0.08375f, (float)EEGetHeight() * 0.2722f, (float)EEGetWidth() * 0.185625f, (float)EEGetHeight() * 0.6467f), starTex, 4);
	starSlide.SetLocalZOrder(9.f);
	selectScene.AddObject(&starSlide);

	//chooseFrame
	EETexture chooseFrameTex(L"Texture/Project Diva Freedom/Song Choosing/Song Choosing-Frame.png");
	EEButton chooseFrameQuad(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.01625f, (float)EEGetHeight() * 0.4033f, (float)EEGetWidth() * 0.33875f, (float)EEGetHeight() * 0.4611f), chooseFrameTex, chooseFrameTex, chooseFrameTex, [&flag] { flag = 1; });
	chooseFrameQuad.SetLocalZOrder(7.f);
	selectScene.AddObject(&chooseFrameQuad);
	//chooseFrame2
	EETexture chooseFrame2Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Song Choosing Frame-2.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Song Choosing Frame-2.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Song Choosing Frame-2.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Song Choosing Frame-2.png",
	};
	EESlide chooseFrame2Slide(Rect_Float((float)EEGetWidth() * 0.01625f, (float)EEGetHeight() * 0.4033f, (float)EEGetWidth() * 0.33875f, (float)EEGetHeight() * 0.4611f), chooseFrame2Tex, 4);
	chooseFrame2Slide.SetLocalZOrder(6.9f);
	selectScene.AddObject(&chooseFrame2Slide);
	//chooseFrame3
	EETexture chooseFrame3Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Song Choosing Frame-3.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Song Choosing Frame-3.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Song Choosing Frame-3.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Song Choosing Frame-3.png",
	};
	EESlide chooseFrame3Slide(Rect_Float((float)EEGetWidth() * 0.01625f, (float)EEGetHeight() * 0.4033f, (float)EEGetWidth() * 0.33875f, (float)EEGetHeight() * 0.4611f), chooseFrame3Tex, 4);
	chooseFrame3Slide.SetLocalZOrder(6.8f);
	selectScene.AddObject(&chooseFrame3Slide);
	//chooseFrame4
	EETexture chooseFrame4Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Song Choosing Frame-4.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Song Choosing Frame-4.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Song Choosing Frame-4.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Song Choosing Frame-4.png",
	};
	EESlide chooseFrame4Slide(Rect_Float((float)EEGetWidth() * 0.01625f, (float)EEGetHeight() * 0.4033f, (float)EEGetWidth() * 0.33875f, (float)EEGetHeight() * 0.4611f), chooseFrame4Tex, 4);
	chooseFrame4Slide.SetLocalZOrder(6.7f);
	selectScene.AddObject(&chooseFrame4Slide);
	//chooseBright
	EETexture chooseBrightTex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Bright（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Bright（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Bright（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Bright（滤色）.png",
	};
	EESlide chooseBrightSlide(Rect_Float((float)EEGetWidth() * 0.f, (float)EEGetHeight() * 0.3944f, (float)EEGetWidth() * 0.57f, (float)EEGetHeight() * 0.4711f), chooseBrightTex, 4);
	chooseBrightSlide.SetLocalZOrder(7.1f);
	selectScene.AddObject(&chooseBrightSlide);

	//ItemsZone
	EETexture itemsZoneTex(L"Texture/Project Diva Freedom/Song Choosing/Items Zone.png");
	EEQuad2D itemsZoneQuad(Rect_Float((float)EEGetWidth() * 0.643125f, (float)EEGetHeight() * 0.0211f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0967f), itemsZoneTex);
	itemsZoneQuad.SetLocalZOrder(5.f);
	selectScene.AddObject(&itemsZoneQuad);

	//ItemsButton
	EETexture itemsButtonTex[2] = {
		L"Texture/Project Diva Freedom/Song Choosing/Items Zone-Button1.png",
		L"Texture/Project Diva Freedom/Song Choosing/Items Zone-Button2.png"
	};
	EEButton itemsButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.649375f, (float)EEGetHeight() * 0.0244f, (float)EEGetWidth() * 0.74875f, (float)EEGetHeight() * 0.0867f), itemsButtonTex[1], itemsButtonTex[1], itemsButtonTex[0]);
	itemsButton.SetLocalZOrder(4.f);
	selectScene.AddObject(&itemsButton);

	//topFrame
	EETexture topFrameTex(L"Texture/Project Diva Freedom/Song Choosing/Top-Frame.png");
	EEQuad2D topFrameQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0456f), topFrameTex);
	topFrameQuad.SetLocalZOrder(5.f);
	selectScene.AddObject(&topFrameQuad);

	//bottomFrame
	EETexture bottomFrameTex(L"Texture/Project Diva Freedom/Song Choosing/Bottom-Frame.png");
	EEQuad2D bottomFrameQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.7922f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), bottomFrameTex);
	bottomFrameQuad.SetLocalZOrder(5.f);
	selectScene.AddObject(&bottomFrameQuad);

	//difficultyFrame
	EETexture difficultyFrameTex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty Choosing-Frame.png");
	EEQuad2D difficultyFrameQuad(Rect_Float((float)EEGetWidth() * 0.510625f, (float)EEGetHeight() * 0.0967f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.1889f), difficultyFrameTex);
	difficultyFrameQuad.SetLocalZOrder(5.f);
	selectScene.AddObject(&difficultyFrameQuad);
	//difficulty2Frame
	EETexture difficultyFrame2Tex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty.png");
	EEQuad2D difficultyFrame2Quad(Rect_Float((float)EEGetWidth() * 0.51375f, (float)EEGetHeight() * 0.1278f, (float)EEGetWidth() * 0.9975f, (float)EEGetHeight() * 0.1822f), difficultyFrame2Tex);
	difficultyFrame2Quad.SetLocalZOrder(4.f);
	selectScene.AddObject(&difficultyFrame2Quad);
	//DifficultyButton
	//EasyButton
	EETexture easyButtonTex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty Easy-Bright.png");
	EEButton easyButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.51f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.638125f, (float)EEGetHeight() * 0.1889f), easyButtonTex, easyButtonTex, easyButtonTex);
	easyButton.SetLocalZOrder(3.f);
	easyButton.SetAlpha(1.0f);
	selectScene.AddObject(&easyButton);
	//NormalButton
	EETexture normalButtonTex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty Normal-Bright.png");
	EEButton normalButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.6325f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.760625f, (float)EEGetHeight() * 0.1889f), normalButtonTex, normalButtonTex, normalButtonTex);
	normalButton.SetLocalZOrder(3.f);
	normalButton.SetAlpha(0.0f);
	selectScene.AddObject(&normalButton);
	//HardButton
	EETexture hardButtonTex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty hard-Bright.png");
	EEButton hardButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.7525f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.880625f, (float)EEGetHeight() * 0.1889f), hardButtonTex, hardButtonTex, hardButtonTex);
	hardButton.SetLocalZOrder(3.f);
	hardButton.SetAlpha(0.0f);
	selectScene.AddObject(&hardButton);
	//ExtremeButton
	EETexture extremeButtonTex(L"Texture/Project Diva Freedom/Song Choosing/Difficulty Extreme-Bright.png");
	EEButton extremeButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.87375f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 1.001875f, (float)EEGetHeight() * 0.1889f), extremeButtonTex, extremeButtonTex, extremeButtonTex);
	extremeButton.SetLocalZOrder(3.f);
	extremeButton.SetAlpha(0.0f);
	selectScene.AddObject(&extremeButton);
	easyButton.SetTriggeredFunc([&difficult, &easyButton, &normalButton, &hardButton, &extremeButton, 
		&bgSlide, &circleSlide, &line1Slide, &line2Slide, &starSlide, 
		&chooseFrame2Slide, &chooseFrame3Slide, &chooseFrame4Slide,
		&chooseBrightSlide]
	{
		difficult = 0;
		easyButton.SetAlpha(1.0f);
		normalButton.SetAlpha(0.0f);
		hardButton.SetAlpha(0.0f);
		extremeButton.SetAlpha(0.0f);

		bgSlide.SetCurrentSlide(difficult);
		circleSlide.SetCurrentSlide(difficult);
		line1Slide.SetCurrentSlide(difficult);
		line2Slide.SetCurrentSlide(difficult);
		starSlide.SetCurrentSlide(difficult);
		chooseFrame2Slide.SetCurrentSlide(difficult);
		chooseFrame3Slide.SetCurrentSlide(difficult);
		chooseFrame4Slide.SetCurrentSlide(difficult);
		chooseBrightSlide.SetCurrentSlide(difficult);
	});
	normalButton.SetTriggeredFunc([&difficult, &easyButton, &normalButton, &hardButton, &extremeButton,
		&bgSlide, &circleSlide, &line1Slide, &line2Slide, &starSlide, 
		&chooseFrame2Slide, &chooseFrame3Slide, &chooseFrame4Slide,
		&chooseBrightSlide]
	{
		difficult = 1;
		easyButton.SetAlpha(0.0f);
		normalButton.SetAlpha(1.0f);
		hardButton.SetAlpha(0.0f);
		extremeButton.SetAlpha(0.0f);

		bgSlide.SetCurrentSlide(difficult);
		circleSlide.SetCurrentSlide(difficult);
		line1Slide.SetCurrentSlide(difficult);
		line2Slide.SetCurrentSlide(difficult);
		starSlide.SetCurrentSlide(difficult);
		chooseFrame2Slide.SetCurrentSlide(difficult);
		chooseFrame3Slide.SetCurrentSlide(difficult);
		chooseFrame4Slide.SetCurrentSlide(difficult);
		chooseBrightSlide.SetCurrentSlide(difficult);
	});
	hardButton.SetTriggeredFunc([&difficult, &easyButton, &normalButton, &hardButton, &extremeButton,
		&bgSlide, &circleSlide, &line1Slide, &line2Slide, &starSlide, 
		&chooseFrame2Slide, &chooseFrame3Slide, &chooseFrame4Slide,
		&chooseBrightSlide]
	{
		difficult = 2;
		easyButton.SetAlpha(0.0f);
		normalButton.SetAlpha(0.0f);
		hardButton.SetAlpha(1.0f);
		extremeButton.SetAlpha(0.0f);

		bgSlide.SetCurrentSlide(difficult);
		circleSlide.SetCurrentSlide(difficult);
		line1Slide.SetCurrentSlide(difficult);
		line2Slide.SetCurrentSlide(difficult);
		starSlide.SetCurrentSlide(difficult);
		chooseFrame2Slide.SetCurrentSlide(difficult);
		chooseFrame3Slide.SetCurrentSlide(difficult);
		chooseFrame4Slide.SetCurrentSlide(difficult);
		chooseBrightSlide.SetCurrentSlide(difficult);
	});
	extremeButton.SetTriggeredFunc([&difficult, &easyButton, &normalButton, &hardButton, &extremeButton,
		&bgSlide, &circleSlide, &line1Slide, &line2Slide, &starSlide, 
		&chooseFrame2Slide, &chooseFrame3Slide, &chooseFrame4Slide,
		&chooseBrightSlide]
	{
		difficult = 3;
		easyButton.SetAlpha(0.0f);
		normalButton.SetAlpha(0.0f);
		hardButton.SetAlpha(0.0f);
		extremeButton.SetAlpha(1.0f);

		bgSlide.SetCurrentSlide(difficult);
		circleSlide.SetCurrentSlide(difficult);
		line1Slide.SetCurrentSlide(difficult);
		line2Slide.SetCurrentSlide(difficult);
		starSlide.SetCurrentSlide(difficult);
		chooseFrame2Slide.SetCurrentSlide(difficult);
		chooseFrame3Slide.SetCurrentSlide(difficult);
		chooseFrame4Slide.SetCurrentSlide(difficult);
		chooseBrightSlide.SetCurrentSlide(difficult);
	});


	while (EERun() && flag == 0)
	{
		EEBeginScene(EEColor::BLACK);

		EEProcess(&selectScene);

		EEEndScene();
	}

	EETimerStop();
	EERemoveThread();
	return flag;
}

int SimpleSet()
{
	int flag = 0;

	//order 10, time 0 - +∞
	EETexture bgTex(L"Texture/Project Diva Freedom/简单设置/背景.jpg");
	EEScene simpleSet(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	simpleSet.SetLocalZOrder(10.0f);

	EETexture infoTex(L"Texture/Project Diva Freedom/简单设置/歌曲信息.png");
	EEQuad2D infoQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.1367f), infoTex);
	infoQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&infoQuad);

	EETexture round1Tex(L"Texture/Project Diva Freedom/简单设置/圈1.png");
	EEQuad2D round1Quad(Rect_Float((float)EEGetWidth() * 0.325625f, (float)EEGetHeight() * 0.1656f, (float)EEGetWidth() * 0.674375f, (float)EEGetHeight() * 0.7856f), round1Tex);
	round1Quad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&round1Quad);

	EETexture round2Tex(L"Texture/Project Diva Freedom/简单设置/圈2（滤色）.png");
	EEQuad2D round2Quad(Rect_Float((float)EEGetWidth() * 0.31f, (float)EEGetHeight() * 0.1378f, (float)EEGetWidth() * 0.69f, (float)EEGetHeight() * 0.8133f), round2Tex);
	round2Quad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&round2Quad);

	EETexture roundBTex(L"Texture/Project Diva Freedom/简单设置/圆底.png");
	EEQuad2D roundBQuad(Rect_Float((float)EEGetWidth() * 0.3225f, (float)EEGetHeight() * 0.16f, (float)EEGetWidth() * 0.6775f, (float)EEGetHeight() * 0.7911f), roundBTex);
	roundBQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&roundBQuad);

	EETexture roundCTex(L"Texture/Project Diva Freedom/简单设置/专辑图遮罩.png");
	EEQuad2D roundCQuad(Rect_Float((float)EEGetWidth() * 0.33625f, (float)EEGetHeight() * 0.1844f, (float)EEGetWidth() * 0.66375f, (float)EEGetHeight() * 0.7667f), roundCTex);
	roundCQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&roundCQuad);

	EETexture start1Tex(L"Texture/Project Diva Freedom/简单设置/开始按钮-1.png");
	EETexture start2Tex(L"Texture/Project Diva Freedom/简单设置/开始按钮-2.png");
	EEButton startButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.394375f, (float)EEGetHeight() * 0.8133f, (float)EEGetWidth() * 0.605625f, (float)EEGetHeight() * 0.9278f), start1Tex, start1Tex, start2Tex, [&flag] { flag = 1; });
	startButton.SetLocalZOrder(9.f);
	simpleSet.AddObject(&startButton);

	EETexture difficultyTex(L"Texture/Project Diva Freedom/简单设置/难度.png");
	EEQuad2D difficultyQuad(Rect_Float((float)EEGetWidth() * -0.00625f, (float)EEGetHeight() * 0.29f, (float)EEGetWidth() * 0.2875f, (float)EEGetHeight() * 0.4267f), difficultyTex);
	difficultyQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&difficultyQuad);

	EETexture scoreTex(L"Texture/Project Diva Freedom/简单设置/最高得分.png");
	EEQuad2D scoreQuad(Rect_Float((float)EEGetWidth() * -0.00625f, (float)EEGetHeight() * 0.4967f, (float)EEGetWidth() * 0.2875f, (float)EEGetHeight() * 0.6556f), scoreTex);
	scoreQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&scoreQuad);

	EETexture evaluateTex(L"Texture/Project Diva Freedom/简单设置/最高评价.png");
	EEQuad2D evaluateQuad(Rect_Float((float)EEGetWidth() * -0.00625f, (float)EEGetHeight() * 0.7033f, (float)EEGetWidth() * 0.2875f, (float)EEGetHeight() * 0.8622f), evaluateTex);
	evaluateQuad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&evaluateQuad);

	EETexture noteTex(L"Texture/Project Diva Freedom/简单设置/NOTE.png");
	EEButton noteButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.705f, (float)EEGetHeight() * 0.3111f, (float)EEGetWidth() * 1.00625f, (float)EEGetHeight() * 0.4711f), noteTex, noteTex, noteTex);
	noteButton.SetLocalZOrder(9.f);
	simpleSet.AddObject(&noteButton);

	EETexture seTex(L"Texture/Project Diva Freedom/简单设置/SE.png");
	EEButton seButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.705f, (float)EEGetHeight() * 0.5233f, (float)EEGetWidth() * 1.00625f, (float)EEGetHeight() * 0.6833f), seTex, seTex, seTex);
	seButton.SetLocalZOrder(9.f);
	simpleSet.AddObject(&seButton);

	EETexture skinTex(L"Texture/Project Diva Freedom/简单设置/SKIN.png");
	EEButton skinButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * 0.705f, (float)EEGetHeight() * 0.7333f, (float)EEGetWidth() * 1.00625f, (float)EEGetHeight() * 0.8933f), skinTex, skinTex, skinTex);
	skinButton.SetLocalZOrder(9.f);
	simpleSet.AddObject(&skinButton);

	EETexture bottom1Tex(L"Texture/Project Diva Freedom/简单设置/底部解释框-底.png");
	EEQuad2D buttom1Quad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.93f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), bottom1Tex);
	buttom1Quad.SetLocalZOrder(9.f);
	simpleSet.AddObject(&buttom1Quad);

	EETexture bottom2Tex(L"Texture/Project Diva Freedom/简单设置/底部解释框-顶.png");
	EEQuad2D bottom2Quad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.9367f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.9933f), bottom2Tex);
	bottom2Quad.SetLocalZOrder(8.f);
	simpleSet.AddObject(&bottom2Quad);

	while (EERun() && flag == 0)
	{
		EEBeginScene(EEColor::WHITE);

		EEProcess(&simpleSet);

		EEEndScene();
	}

	EETimerStop();
	EERemoveThread();
	return flag;
}

int FreePlay()
{
	int flag = 0;

	//bg
	EETexture bgTex(L"Texture/Project Diva Freedom/自由模式/Fantasy Melody St. Logo.png");
	EEScene freePlay(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), bgTex);
	freePlay.SetLocalZOrder(10.0f);

	//top
	EETexture topTex[2] = {
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/HP-Safe Top.png",
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/HP-Danger Top.png"
	};
	EESlide topSlide(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.3233f), topTex, 2);
	topSlide.SetLocalZOrder(9.f);
	freePlay.AddObject(&topSlide);

	EETexture azureTex(L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Azure.png");
	EEQuad2D azureQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 0.056875f, (float)EEGetHeight() * 0.07444f), azureTex);
	azureQuad.SetLocalZOrder(8.f);
	freePlay.AddObject(&azureQuad);

	EETexture menuTex(L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Button.png");
	EEButton menuButton(EE_BUTTON_THREE, Rect_Float((float)EEGetWidth() * -0.000625f, (float)EEGetHeight() * 0.0078f, (float)EEGetWidth() * 0.0525f, (float)EEGetHeight() * 0.0667f), menuTex, menuTex, menuTex);
	menuButton.SetLocalZOrder(7.f);
	freePlay.AddObject(&menuButton);

	EETexture difficultyTex[4] = {
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Difficulty-Easy.png",
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Difficulty-Normal.png",
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Difficulty-Hard.png",
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Difficulty-Normal.png",
	};
	EESlide difficultySlide(Rect_Float((float)EEGetWidth() * -0.000625f, (float)EEGetHeight() * 0.0078f, (float)EEGetWidth() * 0.0525f, (float)EEGetHeight() * 0.0667f), difficultyTex, 4);
	difficultySlide.SetLocalZOrder(9.f);
	freePlay.AddObject(&difficultySlide);

	EETexture bottomTex[2] = {
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/HP-Safe Bottom.png",
		L"Texture/Project Diva Freedom/自由模式/Gaming Interface/HP-Danger Bottom.png"
	};
	EESlide bottomSlide(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.8822f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), bottomTex, 2);
	bottomSlide.SetLocalZOrder(9.f);
	freePlay.AddObject(&bottomSlide);

	EETexture lyricTex(L"Texture/Project Diva Freedom/自由模式/Gaming Interface/lyric.png");
	EEQuad2D lyricQuad(Rect_Float((float)EEGetWidth() * 0.02125f, (float)EEGetHeight() * 0.9033f, (float)EEGetWidth() * 0.08125f, (float)EEGetHeight() * 0.9289f), lyricTex);
	lyricQuad.SetLocalZOrder(8.f);
	freePlay.AddObject(&lyricQuad);

	EETexture progressTex(L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Process Color.png");
	EETexture progressFrameTex(L"Texture/Project Diva Freedom/自由模式/Gaming Interface/Process-Frame.png");
	EEProgressbar progressbar(Rect_Float((float)EEGetWidth() * 0.115625f, (float)EEGetHeight() * 0.9544f, (float)EEGetWidth() * 0.989375f, (float)EEGetHeight() * 0.9856f), progressTex, progressFrameTex);
	progressbar.SetLocalZOrder(8.f);
	freePlay.AddObject(&progressbar);

	//hp
	EETexture hpFrameTex(L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-Frame（线性减淡）.png");
	EEQuad2D hpFrameQuad(Rect_Float((float)EEGetWidth() * 0.004375f, (float)EEGetHeight() * 0.0878f, (float)EEGetWidth() * 0.0725f, (float)EEGetHeight() * 0.2867f), hpFrameTex);
	hpFrameQuad.SetLocalZOrder(8.f);
	freePlay.AddObject(&hpFrameQuad);

	EETexture hpTex[17] = {
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-1.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-2.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-3.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-4.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-5.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-6.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-7.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-8.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-9.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-10.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-11.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-12.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-13.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-14.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-15.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-Full.png",
		L"Texture/Project Diva Freedom/自由模式/HP/HP Variation-Overflow Dull.png",
	};
	EESlide hpSlide(Rect_Float((float)EEGetWidth() * 0.004375f, (float)EEGetHeight() * 0.0878f, (float)EEGetWidth() * 0.0725f, (float)EEGetHeight() * 0.2867f), hpTex, 17);
	hpSlide.SetLocalZOrder(7.f);
	freePlay.AddObject(&hpSlide);

	//pause
	EETexture pauseTex[5] = {
		L"Texture/Project Diva Freedom/自由模式/Count Down/Pause-CountDown-1.png",
		L"Texture/Project Diva Freedom/自由模式/Count Down/Pause-CountDown-2.png",
		L"Texture/Project Diva Freedom/自由模式/Count Down/Pause-CountDown-3.png",
		L"Texture/Project Diva Freedom/自由模式/Count Down/Pause-CountDown-4.png",
		L"Texture/Project Diva Freedom/自由模式/Count Down/Pause-CountDown-5.png",
	};
	EEQuad2D pauseFrameQuad(Rect_Float((float)EEGetWidth() * 0.040625f, (float)EEGetHeight() * 0.3333f, (float)EEGetWidth() * 0.959375f, (float)EEGetHeight() * 0.5833f), pauseTex[0]);
	pauseFrameQuad.SetLocalZOrder(8.f);
	pauseFrameQuad.SetIsAlive(false);
	freePlay.AddObject(&pauseFrameQuad);
	EESlide pauseSlide(Rect_Float((float)EEGetWidth() * 0.28375f, (float)EEGetHeight() * 0.3589f, (float)EEGetWidth() * 0.71625f, (float)EEGetHeight() * 0.5589f), &pauseTex[1], 4);
	pauseSlide.SetLocalZOrder(7.f);
	pauseSlide.SetIsAlive(false);
	freePlay.AddObject(&pauseSlide);

	//ct
	EETexture ctTex[8] = {
		L"Texture/Project Diva Freedom/自由模式/CT/CT(线性减淡).png",
		L"Texture/Project Diva Freedom/自由模式/CT/CT-START.png",
		L"Texture/Project Diva Freedom/自由模式/CT/CT-END.png",
		L"Texture/Project Diva Freedom/自由模式/CT/CT-SUCCESS.png",
		L"Texture/Project Diva Freedom/自由模式/CT/CT-SUCCESS1.png",
		L"Texture/Project Diva Freedom/自由模式/CT/CT-NO SUCCESS.png",
		L"Texture/Project Diva Freedom/自由模式/CT/TinyCT-Picture（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/CT/TinyCT-Words.png",
	};
	EEQuad2D ctFrameQuad(Rect_Float((float)EEGetWidth() * 0.043f, (float)EEGetHeight() * 0.62f, (float)EEGetWidth() * 0.957f, (float)EEGetHeight() * 0.772f), ctTex[0]);
	ctFrameQuad.SetLocalZOrder(8.f);
	ctFrameQuad.SetIsAlive(false);
	freePlay.AddObject(&ctFrameQuad);
	EESlide ctSlide(Rect_Float((float)EEGetWidth() * 0.2619f, (float)EEGetHeight() * 0.661f, (float)EEGetWidth() * 0.7388f, (float)EEGetHeight() * 0.732f), &ctTex[1], 2);
	ctSlide.SetLocalZOrder(7.f);
	ctSlide.SetIsAlive(false);
	freePlay.AddObject(&ctSlide);
	EESlide ctSuccessSlide(Rect_Float((float)EEGetWidth() * 0.29f, (float)EEGetHeight() * 0.424f, (float)EEGetWidth() * 0.71f, (float)EEGetHeight() * 0.576f), &ctTex[3], 2);
	ctSuccessSlide.SetLocalZOrder(7.f);
	ctSuccessSlide.SetIsAlive(false);
	freePlay.AddObject(&ctSuccessSlide);
	//EESlide ctUnsuccessSlide(Rect_Float((float)EEGetWidth() * 0.29f, (float)EEGetHeight() * 0.424f, (float)EEGetWidth() * 0.71f, (float)EEGetHeight() * 0.576f), &ctTex[5], 1);
	//ctUnsuccessSlide.SetLocalZOrder(7.f);
	//freePlay.AddObject(&ctUnsuccessSlide);
	EEQuad2D ctTinyFrameQuad(Rect_Float((float)EEGetWidth() * 0.8713f, (float)EEGetHeight() * 0.1011f, (float)EEGetWidth() * 0.9925f, (float)EEGetHeight() * 0.1578f), ctTex[6]);
	ctTinyFrameQuad.SetLocalZOrder(8.f);
	ctTinyFrameQuad.SetIsAlive(false);
	freePlay.AddObject(&ctTinyFrameQuad);
	EEQuad2D ctTinyQuad(Rect_Float((float)EEGetWidth() * 0.88f, (float)EEGetHeight() * 0.1156f, (float)EEGetWidth() * 0.983125f, (float)EEGetHeight() * 0.1422f), ctTex[7]);
	ctTinyQuad.SetLocalZOrder(7.f);
	ctTinyQuad.SetIsAlive(false);
	freePlay.AddObject(&ctTinyQuad);

	//exit


	while(EERun() && flag == 0)
	{
		EEBeginScene(EEColor::WHITE);

		pauseSlide.SetCurrentSlide(EEGetFPS() % 4);
		hpSlide.SetCurrentSlide(EEGetFPS() % 20);
		EEProcess(&freePlay);

		EEEndScene();
	}

	EERemoveThread();
	return flag;
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
int main(int _argc, char** _argv)
{
	//_CrtSetBreakAlloc(336);
	EEDesc desc;
	desc.applicationName = L"Emerald";	//窗口名称
	desc.isFullScreen = false;			//是否全屏
	desc.width = 800;					//窗口宽度
	desc.height = 450;					//窗口高度
	desc.isSSAA = true;					//是开启抗锯齿
	desc.isVsync = false;				//是否垂直同步
	EEInitialize(desc);
	FreePlay();
	int flag = 0;
	while (flag = MainScene())
	{
		switch (flag)
		{
		case 1:
			while (flag = SelectMusic())
			{
				switch (flag)
				{
				case 1:
					while (flag = SimpleSet())
					{
						switch (flag)
						{
						case 1:
							FreePlay();
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	EEShutdown();
	return 0;
}
#endif