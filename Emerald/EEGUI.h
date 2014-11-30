#pragma once
#ifndef _EE_GUI_H_
#define _EE_GUI_H_

#include <vector>
#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditer
	class EELineEditer : public EEQuad2D
	{
	public:
		EELineEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EELineEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EELineEditer(const EELineEditer& _lineEditer);
		~EELineEditer();

		bool Update();
		bool Render();

	private:
		EEFont m_text;
	};

	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	class EEProgressbar : public EEQuad2D
	{
	public:
		EEProgressbar(const Rect_Float& _progressRect, const Rect_Float& _frameRect, const EETexture& _progressTex, const EETexture& _frameTex, void(*_funcPtr)());
		EEProgressbar(const EEProgressbar& _progressbar);
		~EEProgressbar();

		bool Update();
		bool Render();

		bool SetProgress(float _progress);
		bool SetCallbackFunc(void(*_funcPtr)());

		float GetProgress();
		EETexture* GetProgressTex();
		EETexture* GetFrameTex();

	private:
		//the quad of the frame
		EEQuad2D m_progressFrame;
		//the degree of the progress
		float m_progress;
		bool m_isProgressDirty;
		//callback function
		void(*m_callbackFunc)(void);
	};
	
	//EEButtonType
	//----------------------------------------------------------------------------------------------------
	typedef enum
	{
		EE_BUTTON_THREE = 1,
		EE_BUTTON_SCALE = 2,
	} EEButtonType;

	//EEButton
	//----------------------------------------------------------------------------------------------------
	class EEButton : public EEQuad2D
	{
	public:
		EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _upTex, const EETexture& _overTex, const EETexture& _downTex, void(*_funcPtr)() = NULL);
		EEButton(EEButtonType _type, const Rect_Float& _rect, ID3D11ShaderResourceView* _upTex, ID3D11ShaderResourceView* _overTex, ID3D11ShaderResourceView* _downTex, void(*_funcPtr)() = NULL);
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, void(*_funcPtr)() = NULL);
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, ID3D11ShaderResourceView* _tex, void(*_funcPtr)() = NULL);
		EEButton(const EEButton& _button);
		~EEButton();

		bool Update();
		bool Render();

		Rect_Float GetFinalRect();
		EETexture* GetUpTex();
		EETexture* GetOverTex();
		EETexture* GetDownTex();

	protected:
		virtual bool UpdateObjectState();

		bool Update_THREE();
		bool Render_THREE();
		Rect_Float GetFinalRect_THREE();
		bool Update_SCALE();
		bool Render_SCALE();
		Rect_Float GetFinalRect_SCALE();

	protected:
		//type
		EEButtonType m_type;
		//scale
		float m_aimScale;
		float m_currScaleTime;
		float m_aimScaleTime;
		//alpha
		float m_aimAlpha;
		float m_currFadeTime;
		float m_aimFadeTime;
		//callback function
		void(*m_callbackFunc)(void);
		//the picture of the button
		EETexture m_overTex, m_downTex;
	};

	//EEScne
	//----------------------------------------------------------------------------------------------------
	class EEScene : public EEQuad2D
	{
	public:
		EEScene(const Rect_Float &_rect, const EETexture& _tex);
		EEScene(const Rect_Float &_rect, ID3D11ShaderResourceView *_tex);
		EEScene(const EEScene &_scene);
		~EEScene();

		bool Update();
		bool Render();
		void AddObject(EEObject* _object);

	protected:
		//controls
		std::vector<EEObject*> m_objects;
	};

}

#endif