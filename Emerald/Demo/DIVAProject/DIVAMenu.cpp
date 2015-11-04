#include "DIVAMenu.h"


DIVAMenu::DIVAMenu()
	:
	m_scene(),
	m_backgroundQuad(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight()), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\背景.jpg"),
	m_logoQuad(Rect_Float((float)EEGetWidth() * 0.0325f, (float)EEGetHeight() * 0.3733f, (float)EEGetWidth() * 0.67375f, (float)EEGetHeight() * 0.63222f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\PDF.png"),
	m_circle1Quad(Rect_Float((float)EEGetWidth() * 0.083125f, (float)EEGetHeight() * 0.1456f, (float)EEGetWidth() * 0.434375f, (float)EEGetHeight() * 0.7689f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\圈1.png"),
	m_circle2Quad(Rect_Float((float)EEGetWidth() * -0.00875f, (float)EEGetHeight() * -0.018889f, (float)EEGetWidth() * 0.52625f, (float)EEGetHeight() * 0.93222f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\圈2.png"),
	m_road1Quad(Rect_Float((float)EEGetWidth() * 0.05875f, (float)EEGetHeight() * 0.0922f, (float)EEGetWidth() * 0.70375f, (float)EEGetHeight() * 0.8267f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\线1.png"),
	m_road2Quad(Rect_Float((float)EEGetWidth() * 0.340625f, (float)EEGetHeight() * 0.1333f, (float)EEGetWidth() * 0.658125f, (float)EEGetHeight() * 0.7456f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\线2.png"),
	m_topQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0711f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\顶.png"),
	m_bottomQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.9122f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\底.png"),
	m_dpQuad(Rect_Float((float)EEGetWidth() * 0.726875f, (float)EEGetHeight() * 0.0111f, (float)EEGetWidth() * 0.784375f, (float)EEGetHeight() * 0.0578f), L"Texture\\Project Diva Freedom\\主界面\\默认主题\\dp.png"),
	m_dpBoard(Rect_Float((float)EEGetWidth() * 0.784375f, (float)EEGetHeight() * 0.0111f, (float)EEGetWidth() * 0.984375f, (float)EEGetHeight() * 0.0578f), 8),
	m_freeModeButton(EE_BUTTON_SCALE, Rect_Float((float)EEGetWidth() * 0.03f, (float)EEGetHeight() * 0.8489f, (float)EEGetWidth() * 0.1075f, (float)EEGetHeight() * 0.9856f), 1.3f, 0.2f, 0.2f, L"Texture/Project Diva Freedom\\主界面\\默认主题/自由模式.png")

{
	Initialize();
}

DIVAMenu::~DIVAMenu()
{

}

DIVAState DIVAMenu::Process()
{
	while (EERun())
	{
		EEBeginScene(EEColor::BLACK);

		m_dpBoard.SetValue(EEGetFPS());
		EEProcess(&m_scene);

		EEEndScene();
	}

	return DIVA_END;
}

bool DIVAMenu::Initialize()
{
	m_backgroundQuad.SetLocalZOrder(10.0f);
	m_scene.AddObject(&m_backgroundQuad);

	m_logoQuad.SetLocalZOrder(5.f);
	m_logoQuad.SetAlpha(0.0f);
	EEFade(&m_logoQuad, 1.0f, 1.0f);
	m_scene.AddObject(&m_logoQuad);

	m_circle1Quad.SetLocalZOrder(9.f);
	m_circle1Quad.SetAlpha(0.0f);
	EEFade(&m_circle1Quad, 1.0f, 1.0f, 1.0f);
	EERotateYX(&m_circle1Quad, 25.0f, 1.5f * EE_2PI, 1.0f, true);
	m_scene.AddObject(&m_circle1Quad);

	m_circle2Quad.SetLocalZOrder(9.f);
	m_circle2Quad.SetAlpha(0.0f);
	EEFade(&m_circle2Quad, 1.0f, 1.0f, 2.0f);
	EERotateYX(&m_circle2Quad, 25.0f, -1.5f * EE_2PI, 2.0f, true);
	m_scene.AddObject(&m_circle2Quad);

	m_road1Quad.SetLocalZOrder(8.f);
	m_road1Quad.SetAlpha(0.0f);
	EEFade(&m_road1Quad, 1.0f, 1.0f, 3.5f);
	m_scene.AddObject(&m_road1Quad);

	m_road2Quad.SetLocalZOrder(8.f);
	m_road2Quad.SetAlpha(0.0f);
	EEFade(&m_road2Quad, 1.0f, 1.0f, 3.5f);
	m_scene.AddObject(&m_road2Quad);

	m_topQuad.SetLocalZOrder(7.f);
	m_topQuad.SetAlpha(0.0f);
	EEFade(&m_topQuad, 1.0f, 1.0f, 3.5);
	m_scene.AddObject(&m_topQuad);

	m_bottomQuad.SetLocalZOrder(7.f);
	m_bottomQuad.SetAlpha(0.0f);
	EEFade(&m_bottomQuad, 1.0f, 1.0f, 3.5);
	m_scene.AddObject(&m_bottomQuad);

	m_dpQuad.SetLocalZOrder(6.f);
	m_dpQuad.SetAlpha(0.0f);
	EEFade(&m_dpQuad, 1.0f, 1.0f, 3.5);
	m_scene.AddObject(&m_dpQuad);

	EETexture numTexs[10] = {
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
	m_dpBoard.SetNumTextures(numTexs);
	m_dpBoard.SetLocalZOrder(6.0f);
	m_dpBoard.SetAlpha(0.0f);
	EEFade(&m_dpBoard, 1.0f, 1.0f, 3.5);
	m_scene.AddObject(&m_dpBoard);

	EEParticleInfo info;
	info.amount = 100;
	info.durationTime = 4.f;
	info.deltaDurationTime = 3.f;
	info.position = FLOAT3(830.f, 430.f, 0.0);
	info.deltaPosition = FLOAT3(50.f, 50.f, 0.0);
	info.width = 50;
	info.height = 50;
	info.direction = FLOAT3(-40.0, -30.0f, 0.0);
	info.deltaDirection = FLOAT3(20.0, 30.0f, 0.0);
	info.beginColor = EEColor(1.0f, 1.0f, 1.0f, 1.0f);
	info.deltaBeginColor = EEColor(0.2f, 0.2f, 0.0f, 0.0f);
	info.endColor = EEColor(0.8f, 0.8f, 1.0f, 0.0f);
	info.deltaEndColor = EEColor(0.26f, 0.26f, 0.0f, 0.0f);
	info.beginScale = 1.0f;
	info.deltaBeginScale = 0.0f;
	info.endScale = 4.0f;
	info.deltaEndScale = 0.0f;
	info.texture = EETexture(L"Texture/主界面/随机上升的音符效果（线性减淡）.png");
	m_noteParticle.SetParticleInfo(info);
	m_noteParticle.SetLocalZOrder(9.0f);
	m_scene.AddObject(&m_noteParticle);

	m_freeModeButton.SetLocalZOrder(5.f);
	m_freeModeButton.SetAlpha(0.0f);
	EEFade(&m_freeModeButton, 1.0f, 1.0f, 3.5);
	m_scene.AddObject(&m_freeModeButton);

	return true;
}