#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	void EEComputeFrustumFromProjection(EEFrustum& _frustum, MATRIX& _matrix)
	{

	}

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

	//----------------------------------------------------------------------------------------------------
	bool IntersectTriangle(const FLOAT3& orig, const FLOAT3& dir,
		FLOAT3& v0, FLOAT3& v1, FLOAT3& v2,
		FLOAT* t, FLOAT* u, FLOAT* v)
	{
		// Find vectors for two edges sharing vert0
		FLOAT3 edge1 = v1 - v0;
		FLOAT3 edge2 = v2 - v0;

		// Begin calculating determinant - also used to calculate U parameter
		FLOAT3 pvec = dir.CrossProduct(edge2);

		// If determinant is near zero, ray lies in plane of triangle
		FLOAT det = edge1.DotProduct(pvec);

		FLOAT3 tvec;
		if (det > 0)
		{
			tvec = orig - v0;
		}
		else
		{
			tvec = v0 - orig;
			det = -det;
		}

		if (det < 0.0001f)
			return false;

		// Calculate U parameter and test bounds
		*u = tvec.DotProduct(pvec);
		if (*u < 0.0f || *u > det)
			return false;

		// Prepare to test V parameter
		FLOAT3 qvec = tvec.CrossProduct(edge1);;

		// Calculate V parameter and test bounds
		*v = dir.DotProduct(qvec);
		if (*v < 0.0f || *u + *v > det)
			return false;

		// Calculate t, scale parameters, ray intersects triangle
		*t = edge2.DotProduct(qvec);;
		FLOAT fInvDet = 1.0f / det;
		*t *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;

		return true;
	}
}