#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EECollision(int _x, int _y, int _z, int _w, const POINT& _point)
	{
		return _point.x >= _x && _point.x <= _z && _point.y >= _y && _point.y <= _w;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECollision(const Rect_Int& _rect, const Point& _point)
	{
		return _point.x >= _rect.x && _point.x <= _rect.z && _point.y >= _rect.y && _point.y <= _rect.w;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECollision(const Rect_Float& _rect, const Point& _point)
	{
		return _point.x >= _rect.x && _point.x <= _rect.z && _point.y >= _rect.y && _point.y <= _rect.w;
	}
}