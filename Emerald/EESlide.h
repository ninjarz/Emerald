#pragma once
#ifndef _EE_SLIDE_H_
#define _EE_SLIDE_H_

#include <vector>
#include "EEQuad2D.h"

// memo: textures count <= 128
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	class EESlide : public EEQuad2D
	{
	public:
		EESlide(const Rect_Float& _rect, EETexture& _texs, float _interval = 0.0f, bool _isLoop = true);
		EESlide(const Rect_Float& _rect, EETexture* _texs, int _amount, float _interval = 0.0f, bool _isLoop = true);
		EESlide(const Rect_Float& _rect, std::vector<EETexture>& _texs, float _interval = 0.0f, bool _isLoop = true);
		EESlide(const EESlide& _slide);
		virtual inline EEObject* Clone() { return new EESlide(*this); }

		virtual bool Update();

		bool SetCurrentSlide(int _num);
		void SetInterval(float _interval);
		void SetIsLoop(bool _isLoop);

	protected:
		float m_updateTime;
		float m_changeInterval;
		bool m_isLoop;
	};
}


#endif