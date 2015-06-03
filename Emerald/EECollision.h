#pragma once
#ifndef _EE_COLLISION_H_
#define _EE_COLLISION_H_

#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	struct EEFrustum
	{
		FLOAT3 Origin;              // Origin of the frustum (and projection).
		FLOAT4 Orientation;         // Unit quaternion representing rotation.

		FLOAT RightSlope;           // Positive X slope (X/Z).
		FLOAT LeftSlope;            // Negative X slope.
		FLOAT TopSlope;             // Positive Y slope (Y/Z).
		FLOAT BottomSlope;          // Negative Y slope.
		FLOAT Near, Far;            // Z of the near plane and far plane.
	};

	struct EEAxisAlignedBox
	{
		FLOAT3 center;            // Center of the box.
		FLOAT3 extents;           // Distance from the center to each side.
	};

	//----------------------------------------------------------------------------------------------------
	void EEComputeFrustumFromProjection(EEFrustum& _frustum, MATRIX& _matrix);

	//----------------------------------------------------------------------------------------------------
	bool EECollision(int _x, int _y, int _z, int _w, const Point& _point);
	bool EECollision(const Rect_Int& _rect, const Point& _point);
	bool EECollision(const Rect_Float& _rect, const Point& _point);

	//----------------------------------------------------------------------------------------------------
	
}


#endif