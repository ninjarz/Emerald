#include "DIVAFreeMode.h"


//----------------------------------------------------------------------------------------------------
DIVAFreeMode::DIVAFreeMode()
	:
	m_state(DIVA_DEFAULT),
	m_difficulty(0),
	m_scene(),
	m_backgroundSlide(Rect_Float(0, 0, (float)EEGetWidth(), (float)EEGetHeight())),
	m_circleSlide(Rect_Float((float)EEGetWidth() * -0.2925f, (float)EEGetHeight() * -0.1522f, (float)EEGetWidth() * 0.395f, (float)EEGetHeight() * 1.0711f)),
	m_line1Slide(Rect_Float((float)EEGetWidth() * 0.15f, (float)EEGetHeight() * 0.1556f, (float)EEGetWidth() * 0.509375f, (float)EEGetHeight() * 0.8444f)),
	m_line2Slide(Rect_Float((float)EEGetWidth() * -0.169375f, (float)EEGetHeight() * 0.1056f, (float)EEGetWidth() * 0.56f, (float)EEGetHeight() * 0.9233f)),
	m_starSlide(Rect_Float((float)EEGetWidth() * -0.08375f, (float)EEGetHeight() * 0.2722f, (float)EEGetWidth() * 0.185625f, (float)EEGetHeight() * 0.6467f)),
	m_itemZoneQuad(Rect_Float((float)EEGetWidth() * 0.643125f, (float)EEGetHeight() * 0.0211f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0967f), L"Texture/Project Diva Freedom/Song Choosing/Items Zone.png"),
	m_itemButton(Rect_Float((float)EEGetWidth() * 0.649375f, (float)EEGetHeight() * 0.0244f, (float)EEGetWidth() * 0.74875f, (float)EEGetHeight() * 0.0867f), L"Texture/Project Diva Freedom/Song Choosing/Items Zone-Button2.png", L"Texture/Project Diva Freedom/Song Choosing/Items Zone-Button2.png", L"Texture/Project Diva Freedom/Song Choosing/Items Zone-Button1.png"),
	m_topQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.0f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.0456f), L"Texture/Project Diva Freedom/Song Choosing/Top-Frame.png"),
	m_bottomQuad(Rect_Float((float)EEGetWidth() * 0.0f, (float)EEGetHeight() * 0.7922f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 1.0f), L"Texture/Project Diva Freedom/Song Choosing/Bottom-Frame.png"),
	m_difficultyFrame1Quad(Rect_Float((float)EEGetWidth() * 0.510625f, (float)EEGetHeight() * 0.0967f, (float)EEGetWidth() * 1.0f, (float)EEGetHeight() * 0.1889f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty Choosing-Frame.png"),
	m_difficultyFrame2Quad(Rect_Float((float)EEGetWidth() * 0.51375f, (float)EEGetHeight() * 0.1278f, (float)EEGetWidth() * 0.9975f, (float)EEGetHeight() * 0.1822f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty.png"),
	m_easyButton(Rect_Float((float)EEGetWidth() * 0.51f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.638125f, (float)EEGetHeight() * 0.1889f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty Easy-Bright.png"),
	m_normalButton(Rect_Float((float)EEGetWidth() * 0.6325f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.760625f, (float)EEGetHeight() * 0.1889f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty Normal-Bright.png"),
	m_hardButton(Rect_Float((float)EEGetWidth() * 0.7525f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 0.880625f, (float)EEGetHeight() * 0.1889f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty hard-Bright.png"),
	m_extremeButton(Rect_Float((float)EEGetWidth() * 0.87375f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() * 1.001875f, (float)EEGetHeight() * 0.1889f), L"Texture/Project Diva Freedom/Song Choosing/Difficulty Extreme-Bright.png"),
	m_listView(Rect_Float((float)EEGetWidth() * 0.87375f, (float)EEGetHeight() * 0.1211f, (float)EEGetWidth() / 2, (float)EEGetHeight() / 2))
{
	Initialize();
}

//----------------------------------------------------------------------------------------------------
DIVAFreeMode::~DIVAFreeMode()
{

}

//----------------------------------------------------------------------------------------------------
DIVAState DIVAFreeMode::Process()
{
	while (EERun() && m_state == DIVA_DEFAULT)
	{
		EEBeginScene(EEColor::BLACK);

		EEProcess(&m_scene);

		EEEndScene();
	}

	Shutdown();

	return m_state;
}

//----------------------------------------------------------------------------------------------------
bool DIVAFreeMode::Initialize()
{
	EETexture backgroundTex[4] = {
		L"Texture\\Project Diva Freedom\\Song Choosing\\Easy\\Easy-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Normal\\Normal-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Hard\\Hard-Background.jpg",
		L"Texture\\Project Diva Freedom\\Song Choosing\\Extreme\\Extreme-Background.jpg",
	};
	m_backgroundSlide.SetTexture(backgroundTex, 4);
	m_backgroundSlide.SetLocalZOrder(9.9f);
	m_scene.AddObject(&m_backgroundSlide);

	EETexture circleTex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Shiny Circle（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Shiny Circle（颜色减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Shiny Circle（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Shiny Circle（颜色减淡）.png",
	};
	m_circleSlide.SetTexture(circleTex, 4);
	m_circleSlide.SetLocalZOrder(9.f);
	EERotateYX(&m_circleSlide, 25.0f, 1.5f * EE_2PI, 1.0f, true);
	m_scene.AddObject(&m_circleSlide);

	EETexture line1Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Line（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Line（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Line（滤色）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Line（滤色）.png",
	};
	m_line1Slide.SetTexture(line1Tex, 4);
	m_line1Slide.SetLocalZOrder(8.f);
	m_scene.AddObject(&m_line1Slide);

	EETexture line2Tex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Line2（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Line2（滤色）.png",
	};
	m_line2Slide.SetTexture(line2Tex, 4);
	m_line2Slide.SetLocalZOrder(8.f);
	m_scene.AddObject(&m_line2Slide);

	EETexture starTex[4] = {
		L"Texture/Project Diva Freedom/Song Choosing/Easy/Easy-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Normal/Normal-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Hard/Hard-Star（线性减淡）.png",
		L"Texture/Project Diva Freedom/Song Choosing/Extreme/Extreme-Star（线性减淡）.png",
	};
	m_starSlide.SetTexture(starTex, 4);
	m_starSlide.SetLocalZOrder(9.f);
	m_scene.AddObject(&m_starSlide);

	m_itemZoneQuad.SetLocalZOrder(5.f);
	m_scene.AddObject(&m_itemZoneQuad);

	m_itemButton.SetLocalZOrder(4.f);
	m_scene.AddObject(&m_itemButton);

	m_topQuad.SetLocalZOrder(5.f);
	m_scene.AddObject(&m_topQuad);

	m_bottomQuad.SetLocalZOrder(5.f);
	m_scene.AddObject(&m_bottomQuad);

	m_difficultyFrame1Quad.SetLocalZOrder(5.f);
	m_scene.AddObject(&m_difficultyFrame1Quad);
	
	m_difficultyFrame2Quad.SetLocalZOrder(4.f);
	m_scene.AddObject(&m_difficultyFrame2Quad);

	m_easyButton.SetLocalZOrder(3.f);
	m_easyButton.SetAlpha(1.0f);
	m_easyButton.SetTriggeredFunc([this]
	{
		SetDifficulty(0);
	});
	m_scene.AddObject(&m_easyButton);

	m_normalButton.SetLocalZOrder(3.f);
	m_normalButton.SetAlpha(0.0f);
	m_normalButton.SetTriggeredFunc([this]
	{
		SetDifficulty(1);
	});
	m_scene.AddObject(&m_normalButton);
	
	m_hardButton.SetLocalZOrder(3.f);
	m_hardButton.SetAlpha(0.0f);
	m_hardButton.SetTriggeredFunc([this]
	{
		SetDifficulty(2);
	});
	m_scene.AddObject(&m_hardButton);
	
	m_extremeButton.SetLocalZOrder(3.f);
	m_extremeButton.SetAlpha(0.0f);
	m_extremeButton.SetTriggeredFunc([this]
	{
		SetDifficulty(3);
	});
	m_scene.AddObject(&m_extremeButton);

	
	m_scene.AddObject(&m_listView);

	return true;
}

//----------------------------------------------------------------------------------------------------
void DIVAFreeMode::Shutdown()
{

}

//----------------------------------------------------------------------------------------------------
bool DIVAFreeMode::SetDifficulty(int _difficulty)
{
	switch (_difficulty)
	{
	case 0:
		m_easyButton.SetAlpha(1.0f);
		m_normalButton.SetAlpha(0.0f);
		m_hardButton.SetAlpha(0.0f);
		m_extremeButton.SetAlpha(0.0f);
		break;
	case 1:
		m_easyButton.SetAlpha(0.0f);
		m_normalButton.SetAlpha(1.0f);
		m_hardButton.SetAlpha(0.0f);
		m_extremeButton.SetAlpha(0.0f);
		break;
	case 2:
		m_easyButton.SetAlpha(0.0f);
		m_normalButton.SetAlpha(0.0f);
		m_hardButton.SetAlpha(1.0f);
		m_extremeButton.SetAlpha(0.0f);
		break;
	case 3:
		m_easyButton.SetAlpha(0.0f);
		m_normalButton.SetAlpha(0.0f);
		m_hardButton.SetAlpha(0.0f);
		m_extremeButton.SetAlpha(1.0f);
		break;
	default:
		return false;
	}
	m_difficulty = _difficulty;
	m_backgroundSlide.SetCurrentSlide(m_difficulty);
	m_circleSlide.SetCurrentSlide(m_difficulty);
	m_line1Slide.SetCurrentSlide(m_difficulty);
	m_line2Slide.SetCurrentSlide(m_difficulty);
	m_starSlide.SetCurrentSlide(m_difficulty);

	return true;
}