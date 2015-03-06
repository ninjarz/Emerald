#pragma once
#ifndef _EE_ANIMATION_2D_H_
#define _EE_ANIMATION_2D_H_

#include <utility>
#include "EEGeometry.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAnimation2D
	class EEAnimation2D : public EEObject
	{
	public:
		bool Update();
		bool Render();

		bool Start();
		bool AddFrame(float _time, EEObject _object);


	protected:
		std::vector<std::pair<float, EEObject>> m_frames;
		float m_startTime;

	};
}


#endif