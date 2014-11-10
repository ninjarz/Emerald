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
	void EEMoveBy(EEObject* _object, float _time, const FLOAT2& _dir);
	void EEMoveByProcess(EEObject* _object, float _time, const FLOAT2& _dir, float _startTime);

	void EEScale(EEObject* _object, float _time, float _degree);
	void EEScaleProcess(EEObject* _object, float _time, float _degree, float _startTime);

	void EERotate(EEObject* _object, float _time, float _radians);
	void EERotateProcess(EEObject* _object, float _time, float _radians, float _startTime);

	void EERotateBy(EEObject* _object, float _time, float _radians, const FLOAT2& _center);
	void EERotateByProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _startTime);
}


#endif