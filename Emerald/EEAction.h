#pragma once
#ifndef _EE_ACTION_H_
#define _EE_ACTION_H_

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "EEMath.h"
#include "EEGUI.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAction_APIs
	//----------------------------------------------------------------------------------------------------
	boost::thread* EEMoveBy(EEObject* _object, float _time, const FLOAT2& _dir, float _delay = 0.0f);
	void EEMoveByProcess(EEObject* _object, float _time, const FLOAT2& _dir, float _delay, float _startTime);

	boost::thread* EEScale(EEObject* _object, float _time, float _degree, float _delay = 0.0f);
	void EEScaleProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime);

	boost::thread* EERotate(EEObject* _object, float _time, float _radians, float _delay = 0.0f);
	void EERotateProcess(EEObject* _object, float _time, float _radians, float _delay, float _startTime);

	boost::thread* EERotate(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay = 0.0f);
	void EERotateProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center,float _delay, float _startTime);

	boost::thread* EERotateBy(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay = 0.0f);
	void EERotateByProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay, float _startTime);

	boost::thread* EEFade(EEObject* _object, float _time, float _degree, float _delay = 0.0f);
	void EEFadeProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime);
}


#endif