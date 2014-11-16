#pragma once
#ifndef _EE_GUI_H_
#define _EE_GUI_H_

#include <vector>
#include "EEGeometry.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	class EEProgressbar : public EEQuad
	{
	public:
		EEProgressbar(const Rect_Float& _quadRect, const Rect_Float& _progressRect, const EETexture& _progressTex, const EETexture& _borderTex, void(*_funcPtr)());
		EEProgressbar(const EEProgressbar& _progressbar);
		~EEProgressbar();

		bool Update();
		bool Render();

		bool SetProgress(float _progress);
		bool SetCallbackFunc(void(*_funcPtr)());

		float GetProgress();
		EETexture* GetProgressTex();
		EETexture* GetBorderTex();

	private:
		bool CreateProgressVertexBuffer();

	private:
		//the size of the progress
		Rect_Float m_progressRect;
		float m_progressWidth, m_progressHeight;
		//the degree of the progress
		float m_progress;
		bool m_isProgressDirty;
		//the vertex buffer of the progress
		ID3D11Buffer *m_progressVB;
		//callback function
		void(*m_callbackFunc)(void);
		//border texture
		EETexture m_progressTex;
	};
	
	//EEButtonType
	//----------------------------------------------------------------------------------------------------
	typedef enum
	{
		EE_BUTTON_THREE = 1,
		EE_BUTTON_SCALE = 2,
	} EEButtonType;

	//EEButtonState
	//----------------------------------------------------------------------------------------------------
	typedef enum
	{
		EE_BUTTON_UP = 1,
		EE_BUTTON_DOWN = 2,
		EE_BUTTON_OVER = 3,
	} EEButtonState;

	//EEButton
	//----------------------------------------------------------------------------------------------------
	class EEButton : public EEQuad
	{
	public:
		EEButton(EEButtonType _type, const Rect_Float& _rect, const EETexture& _upTex, const EETexture& _overTex, const EETexture& _downTex, void(*_funcPtr)());
		EEButton(EEButtonType _type, const Rect_Float& _rect, ID3D11ShaderResourceView* _upTex, ID3D11ShaderResourceView* _overTex, ID3D11ShaderResourceView* _downTex, void(*_funcPtr)());
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, const EETexture& _tex, void(*_funcPtr)());
		EEButton(EEButtonType _type, const Rect_Float& _rect, float _scale, float _scaleTime, float _fadeTime, ID3D11ShaderResourceView* _tex, void(*_funcPtr)());
		EEButton(const EEButton& _button);
		~EEButton();

		bool Update();
		bool Render();

		EETexture* GetUpTex();
		EETexture* GetOverTex();
		EETexture* GetDownTex();

	private:
		bool UpdateButtonState();

		bool Update_THREE();
		bool Render_THREE();
		bool Update_SCALE();
		bool Render_SCALE();

	private:
		//type
		EEButtonType m_type;
		//state
		EEButtonState m_state;
		bool m_isTriggered;
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
		//button picture
		EETexture m_overTex, m_downTex;
	};

	//EEScne
	//----------------------------------------------------------------------------------------------------
	class EEScene : public EEQuad
	{
	public:
		EEScene(const Rect_Float &_rect, const EETexture& _tex);
		EEScene(const Rect_Float &_rect, ID3D11ShaderResourceView *_tex);
		EEScene(const EEScene &_scene);
		~EEScene();

		bool Update();
		bool Render();
		void AddObject(EEObject* _object);

	private:
		//controls
		std::vector<EEObject*> m_objects;
	};

}

#endif