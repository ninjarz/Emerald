#pragma once
#ifndef _EE_ACTION_H_
#define _EE_ACTION_H_

#include "EEThread.h"
#include "EEMath.h"
#include "EEGUI.h"

//memo:save the return value of actions to object
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAction_APIs
	//----------------------------------------------------------------------------------------------------
	boost::thread* EEMoveBy(EEObject* _object, float _time, const FLOAT2& _dir, float _delay = 0.0f, bool _isInfinite = false);
	void EEMoveByProcess(EEObject* _object, float _time, const FLOAT2& _dir, float _delay, bool _isInfinite, float _startTime);

	boost::thread* EEScale(EEObject* _object, float _time, float _degree, float _delay = 0.0f);
	void EEScaleProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime);

	boost::thread* EERotateYX(EEObject* _object, float _time, float _radiansYX, float _delay = 0.0f, bool _isInfinite = false);
	void EERotateYXProcess(EEObject* _object, float _time, float _radiansYX, float _delay, bool _isInfinite, float _startTime);

	boost::thread* EERotate(EEObject* _object, float _time, float _radians, const FLOAT3& _center, float _delay = 0.0f, bool _isInfinite = false);
	void EERotateProcess(EEObject* _object, float _time, float _radians, const FLOAT3& _center, float _delay, bool _isInfinite, float _startTime);

	boost::thread* EERotateBy(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay = 0.0f, bool _isInfinite = false);
	void EERotateByProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay, bool _isInfinite, float _startTime);

	boost::thread* EEFade(EEObject* _object, float _time, float _degree, float _delay = 0.0f);
	void EEFadeProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime);
}


#endif