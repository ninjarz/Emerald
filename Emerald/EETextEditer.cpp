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
		m_font(FLOAT3(_rect.x, _rect.y, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EETextEditer::EETextEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_font(FLOAT3(_rect.x, _rect.y, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EETextEditer::EETextEditer(const EETextEditer& _lineEditer)
		:
		EEQuad2D(_lineEditer),
		m_font(_lineEditer.m_font)
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

		if (s_focusedObject == this)
		{
			//the key queue need to be clear here
		}
		if (s_triggeredObject == this)
		{
			if (EECore::s_EECore->IsKeyInput())
			{
				m_font.AddText((wchar_t)EECore::s_EECore->GetKey());
			}
		}
		m_font.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETextEditer::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		m_font.Render();

		return true;
	}
}