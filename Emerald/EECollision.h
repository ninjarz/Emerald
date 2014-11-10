#pragma once
#ifndef _EE_COLLISION_H_
#define _EE_COLLISION_H_

#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EECollision(int _x, int _y, int _z, int _w, const Point& _point);
	bool EECollision(const Rect_Int& _rect, const Point& _point);
	bool EECollision(const Rect_Float& _rect, const Point& _point);
}


#endif