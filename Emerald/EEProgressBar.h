#pragma once
#ifndef _EE_PROGRESSBAR_H_
#define _EE_PROGRESSBAR_H_

#include <functional>
#include "EEGeometry.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEProgressBar
	//----------------------------------------------------------------------------------------------------
	class EEProgressbar : public EEQuad2D
	{
	public:
		EEProgressbar(const Rect_Float& _progressRect, const EETexture& _progressTex, const EETexture& _frameTex, std::function<void(void)> _funcPtr = []{});
		EEProgressbar(const Rect_Float& _progressRect, const Rect_Float& _frameRect, const EETexture& _progressTex, const EETexture& _frameTex, std::function<void(void)> _funcPtr = []{});
		EEProgressbar(const EEProgressbar& _progressbar);
		~EEProgressbar();

		bool Update();
		bool Render();

		//localZOrder
		virtual void SetLocalZOrder(float _localZOrder);
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
		//void(*m_callbackFunc)(void);
		std::function<void(void)> m_callbackFunc;
	};
}

#endif