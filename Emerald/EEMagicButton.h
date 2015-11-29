#pragma once
#ifndef _EE_MAGICBUTTON_H_
#define _EE_MAGICBUTTON_H_

#include "EEQuad2D.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEButton
	//----------------------------------------------------------------------------------------------------
	class EEMagicButton : public EEQuad2D
	{
	public:
		EEMagicButton(const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, std::function<void(void)> _funcPtr = [] {});
		EEMagicButton(const EEMagicButton& _button);
		virtual ~EEMagicButton();

		virtual bool Update();
		virtual bool Render();

		virtual Rect_Float GetFinalRect() const;

		// OnFunction
		virtual void OnMouseFree(const Point& _pos);
		virtual void OnMouseOver(const Point& _pos);
		virtual void OnMouseClicked(const Point& _pos);
		virtual void OnMouseTriggered(const Point& _pos);

	protected:
		// scale
		float m_aimScale;
		float m_currScaleTime;
		float m_aimScaleTime;
		// alpha
		float m_aimAlpha;
		float m_currFadeTime;
		float m_aimFadeTime;
		// callback function
		std::function<void(void)> m_callbackFunc;
	};
}

#endif