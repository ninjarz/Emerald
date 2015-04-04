#pragma once
#ifndef _EE_ANIMATION_H_
#define _EE_ANIMATION_H_

#include <queue>
#include <functional>
#include "EEGeometry.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAnimationFrame
	//----------------------------------------------------------------------------------------------------
	struct EEAnimationFrame
	{
		bool isRunning;
		int id;
		EEObject *object;
		float duration;
		std::vector<std::function<boost::thread*(EEObject*, float)>> actions;

		inline EEAnimationFrame() : isRunning(false), id(-1), object(nullptr), actions() {}
		inline EEAnimationFrame(const EEAnimationFrame& _frame) { *this = _frame; }
		inline ~EEAnimationFrame()
		{
			SAFE_DELETE(object);
		}

		EEAnimationFrame& operator= (const EEAnimationFrame& _frame)
		{
			isRunning = _frame.isRunning;
			id = _frame.id;
			object = _frame.object->Clone();
			duration = _frame.duration;
			actions = _frame.actions;

			return *this;
		}
	};

	//EEAnimation
	//----------------------------------------------------------------------------------------------------
	class EEAnimation : public EEObject
	{
	public:
		EEAnimation();
		virtual inline EEObject* Clone() { return new EEAnimation(*this); }

		virtual bool Update();
		virtual bool Render();

		bool Start();
		bool AddFrame(const EEAnimationFrame& _frame);

	protected:
		std::queue<EEAnimationFrame*> m_frames;
		std::vector<EEAnimationFrame*> m_backup;
		float m_startTime;
		bool m_isLoop;
	};
}


#endif