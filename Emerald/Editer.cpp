//Animation Demo
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

	EETexture hitTex[34] = {
		L"Texture/Project Diva Freedom/自由模式/HIT/effect30.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect29.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect28.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect27.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect26.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect25.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect24.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect23.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect22.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect21.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect20.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect19.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect18.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect17.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect16.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect15.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect14.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect13.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect12.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect11.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect10.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect9.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect8.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect7.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect6.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect5.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect4.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect3.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect2.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-1（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-2（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-3（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-4（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/sad-01.png",
	};
	// 0
	EESlide hitSlide(Rect_Float(-64, -64, 64, 64), hitTex, 29, 0.0f, false);
	hitSlide.SetInterval(0.05f / 29.f);
	
	EEAnimationFrame frame0;
	frame0.SetObject(hitSlide);
	frame0.duration = 0.05f;

	// 1
	EEQuad2D quad1(Rect_Float(-40, -40, 40, 40), hitTex[29]);
	//quad1.SetScale(3.0f);
	EEAnimationFrame frame1;
	frame1.SetObject(quad1);
	frame1.duration = 0.35f;
	float alpha11 = 0.0f;
	float scale11 = 2.0f;
	frame1.actions.push_back(boost::bind(EEFade, _1, 0.35f, alpha11, _2));
	frame1.actions.push_back(boost::bind(EEScale, _1, 0.35f, scale11, _2));

	// 2
	EEQuad2D quad2(Rect_Float(-40, -40, 40, 40), hitTex[30]);
	EEAnimationFrame frame2;
	frame2.SetObject(quad2);
	frame2.duration = 0.05f;
	float rotation2 = EE_2PI;
	frame2.actions.push_back(boost::bind(EERotateYX, _1, 0.05f, rotation2, _2, false));

	// 3
	EEQuad2D quad3(Rect_Float(-50, -50, 50, 50), hitTex[31]);
	float alpha30 = 0.8f;
	float alpha31 = 0.0f;
	quad3.SetAlpha(alpha30);
	EEAnimationFrame frame3;
	frame3.SetObject(quad3);
	frame3.duration = 0.05f;
	frame3.actions.push_back(boost::bind(EEFade, _1, 0.05f, alpha31, _2));

	// 4
	EEQuad2D quad4(Rect_Float(-50, -50, 50, 50), hitTex[32]);
	EEAnimationFrame frame4;
	frame4.SetObject(quad4);
	frame4.duration = 0.07f;
	float alpha41 = 0.0f;
	float scale41 = 4.0f;
	frame4.actions.push_back(boost::bind(EEFade, _1, 0.07f, alpha41, _2));
	frame4.actions.push_back(boost::bind(EEScale, _1, 0.07f, scale41, _2));

	EEAnimation animation;
	animation.AddFrame(frame0);
	animation.AddFrame(frame2);
	animation.AddFrame(frame3);
	animation.AddFrame(frame1);
	animation.AddFrame(frame4);
	//animation.SetIsLoop(true);

	EEAnimationEmitter *emitter = new EEAnimationEmitter();
	emitter->SetAnimation(&animation);
	std::vector<EEAnimationFrame*> target = emitter->GetAnimation()->GetFrames();

	EETexture musicFrameTex(L"Texture/主界面/播放器/时间轴.png");
	EETexture musicProgressTex(L"Texture/主界面/播放器/进度.png");
	EEProgressbar progressbar1(Rect_Float(520.f, 305.f, 700.f, 310.f), musicProgressTex, musicFrameTex);
	progressbar1.SetIsAlive(false);
	EEProgressbar progressbar2(Rect_Float(520.f, 330.f, 700.f, 335.f), musicProgressTex, musicFrameTex);
	progressbar2.SetIsAlive(false);
	EEProgressbar progressbar3(Rect_Float(520.f, 355.f, 700.f, 360.f), musicProgressTex, musicFrameTex);
	progressbar3.SetIsAlive(false);
	EELineBrowser *lineBrowser1 = new EELineBrowser(Rect_Float(250.f, 300.f, 500.f, 320.f), EEColor::WHITE, EEColor::BLACK);
	EELineBrowser *lineBrowser2 = new EELineBrowser(Rect_Float(250.f, 320.f, 500.f, 340.f), EEColor::WHITE, EEColor::BLACK);
	EELineBrowser *lineBrowser3 = new EELineBrowser(Rect_Float(250.f, 340.f, 500.f, 360.f), EEColor::WHITE, EEColor::BLACK);
	EEComboBox *combo = new EEComboBox(Rect_Float(100.f, 300.f, 200.f, 320.f), EEColor::WHITE, EEColor::BLACK);
	combo->AddItem(L"小圆:effect", 
		[&]
	{
		progressbar1.SetIsAlive(true);
		progressbar1.SetProgress(target[0]->duration);
		progressbar1.SetCallbackFunc(
			[&](float _x)
		{ 
			target[0]->duration = _x;
			EESlide *object = (EESlide*)target[0]->object;
			object->SetInterval(_x / object->GetTexture()->GetNumber());
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << _x;
			ss >> degree;
			lineBrowser1->SetText(std::wstring(L"持续时间：") + degree);
		});
		progressbar2.SetIsAlive(false);
		lineBrowser2->SetText(L"");
		progressbar3.SetIsAlive(false);
		lineBrowser3->SetText(L"");
	});
	combo->AddItem(L"hit2",
		[&]
	{
		progressbar1.SetIsAlive(true);
		progressbar1.SetProgress(target[1]->duration);
		progressbar1.SetCallbackFunc(
			[&](float _x)
		{
			target[1]->duration = _x;
			target[1]->actions[0] = boost::bind(EERotateYX, _1, _x, rotation2, _2, false);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << _x;
			ss >> degree;
			lineBrowser1->SetText(std::wstring(L"持续时间：") + degree);
		});
		progressbar2.SetIsAlive(true);
		progressbar2.SetProgress(rotation2 / (EE_2PI * 4));
		progressbar2.SetCallbackFunc(
			[&](float _x)
		{
			rotation2 = _x * (EE_2PI * 4);
			target[1]->actions[0] = boost::bind(EERotateYX, _1, target[1]->duration, rotation2, _2, false);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << rotation2;
			ss >> degree;
			lineBrowser2->SetText(std::wstring(L"旋转：") + degree);
		});
		progressbar3.SetIsAlive(false);
		lineBrowser3->SetText(L"");
	});
	combo->AddItem(L"hit3",
		[&]
	{
		progressbar1.SetIsAlive(true);
		progressbar1.SetProgress(target[2]->duration);
		progressbar1.SetCallbackFunc(
			[&](float _x)
		{
			target[2]->duration = _x;
			target[2]->actions[0] = boost::bind(EEFade, _1, target[2]->duration, alpha11, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << _x;
			ss >> degree;
			lineBrowser1->SetText(std::wstring(L"持续时间：") + degree);
		});
		progressbar2.SetIsAlive(true);
		progressbar2.SetProgress(alpha30);
		progressbar2.SetCallbackFunc(
			[&](float _x)
		{
			alpha30 = _x;
			EEQuad2D *object = (EEQuad2D*)target[2]->object;
			object->SetAlpha(alpha30);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << alpha30;
			ss >> degree;
			lineBrowser2->SetText(std::wstring(L"起始alpha：") + degree);
		});
		progressbar3.SetIsAlive(true);
		progressbar3.SetProgress(alpha31);
		progressbar3.SetCallbackFunc(
			[&](float _x)
		{
			alpha31 = _x;
			target[2]->actions[0] = boost::bind(EEFade, _1, target[2]->duration, alpha31, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << alpha31;
			ss >> degree;
			lineBrowser3->SetText(std::wstring(L"结束alpha：") + degree);
		});
	});
	combo->AddItem(L"hit1",
		[&]
	{
		progressbar1.SetIsAlive(true);
		progressbar1.SetProgress(target[3]->duration);
		progressbar1.SetCallbackFunc(
			[&](float _x)
		{
			target[3]->duration = _x;
			target[3]->actions[0] = boost::bind(EEFade, _1, target[3]->duration, alpha11, _2);
			target[3]->actions[1] = boost::bind(EEScale, _1, target[3]->duration, scale11, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << _x;
			ss >> degree;
			lineBrowser1->SetText(std::wstring(L"持续时间：") + degree);
		});
		progressbar2.SetIsAlive(true);
		progressbar2.SetProgress(alpha11);
		progressbar2.SetCallbackFunc(
			[&](float _x)
		{
			alpha11 = _x;
			target[3]->actions[0] = boost::bind(EEFade, _1, target[3]->duration, alpha11, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << alpha11;
			ss >> degree;
			lineBrowser2->SetText(std::wstring(L"结束alpha：") + degree);
		});
		progressbar3.SetIsAlive(true);
		progressbar3.SetProgress(scale11 / 5.f);
		progressbar3.SetCallbackFunc(
			[&](float _x)
		{
			scale11 = _x * 5.f;
			target[3]->actions[1] = boost::bind(EEScale, _1, target[3]->duration, scale11, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << scale11;
			ss >> degree;
			lineBrowser3->SetText(std::wstring(L"结束scale：") + degree);
		});
	});
	combo->AddItem(L"hit4",
		[&]
	{
		progressbar1.SetIsAlive(true);
		progressbar1.SetProgress(target[4]->duration);
		progressbar1.SetCallbackFunc(
			[&](float _x)
		{
			target[4]->duration = _x;
			target[4]->actions[0] = boost::bind(EEFade, _1, target[4]->duration, alpha41, _2);
			target[4]->actions[1] = boost::bind(EEScale, _1, target[4]->duration, scale41, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << _x;
			ss >> degree;
			lineBrowser1->SetText(std::wstring(L"持续时间：") + degree);
		});
		progressbar2.SetIsAlive(true);
		progressbar2.SetProgress(alpha41);
		progressbar2.SetCallbackFunc(
			[&](float _x)
		{
			alpha41 = _x;
			target[4]->actions[0] = boost::bind(EEFade, _1, target[4]->duration, alpha41, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << alpha41;
			ss >> degree;
			lineBrowser2->SetText(std::wstring(L"结束alpha：") + degree);
		});
		progressbar3.SetIsAlive(true);
		progressbar3.SetProgress(scale41 / 5.f);
		progressbar3.SetCallbackFunc(
			[&](float _x)
		{
			scale41 = _x * 5.f;
			target[4]->actions[1] = boost::bind(EEScale, _1, target[4]->duration, scale41, _2);
			emitter->SetIsAnimationDirty(true);
			std::wstringstream ss;
			std::wstring degree;
			ss << scale41;
			ss >> degree;
			lineBrowser3->SetText(std::wstring(L"结束scale：") + degree);
		});
	});

	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);
		//EEShowFPSInTitle(L"- -");

		combo->Process();
		progressbar1.Process();
		progressbar2.Process();
		progressbar3.Process();
		lineBrowser1->Process();
		lineBrowser2->Process();
		lineBrowser3->Process();
		if (EEIsMouseInput())
		{
			EEGetMouse();
			emitter->Emit(FLOAT3(EEGetMousePosition(), 0.0f));
		}
		emitter->Process();

		//printf("%d ", EEGetFPS());
		EEEndScene();
	}
	SAFE_DELETE(emitter);

	EEShutdown();
	return 0;
}

#endif