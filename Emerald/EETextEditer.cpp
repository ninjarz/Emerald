#include "EETextEditer.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETextEditer
	//----------------------------------------------------------------------------------------------------
	EETextEditer::EETextEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _color),
		m_text(FLOAT3(_rect.x, _rect.y, 0.0f), _fontColor, L"")
	{
		m_text.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EETextEditer::EETextEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_text(FLOAT3(_rect.x, _rect.y, 0.0f), _fontColor, L"")
	{
		m_text.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EETextEditer::EETextEditer(const EETextEditer& _lineEditer)
		:
		EEQuad2D(_lineEditer),
		m_text(_lineEditer.m_text)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETextEditer::~EETextEditer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EETextEditer::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (m_isTriggered)
		{
			//the key queue need to be clear here
			m_isTriggered = false;
		}
		if (s_focusedObject == this)
		{
			if (EECore::s_EECore->IsKeyInput())
			{
				m_text.AddText((char)EECore::s_EECore->GetKey());
			}
		}
		m_text.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETextEditer::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		m_text.Render();

		return true;
	}
}