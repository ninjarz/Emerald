#include "EEMath.h"
#include <algorithm>

namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	const MATRIX MATRIX::ZERO(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0);

	//----------------------------------------------------------------------------------------------------
	const MATRIX MATRIX::IDENTITY(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	//----------------------------------------------------------------------------------------------------
	FLOAT2 EEBezier(const FLOAT2& _p0, const FLOAT2& _p1, const FLOAT2& _p2, float _t)
	{
		float tt = 1 - _t;
		return _p0 * tt * tt + _p1 * 2 * _t * tt + _p2 * _t * _t;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT2 EEBezier(const FLOAT2& _p0, const FLOAT2& _p1, const FLOAT2& _p2, const FLOAT2& _p3, float _t)
	{
		float tt = 1 - _t;
		return _p0 * (tt * tt * tt) + _p1 * (3 * _t * tt * tt) + _p2 * (3 * _t * _t * tt) + _p3 * (_t * _t * _t);
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT2 EEBSpline(const FLOAT2& _p0, const FLOAT2& _p1, const FLOAT2& _p2, float _t)
	{
		float tt = _t * _t;
		return (_p0 * (1 - 2 * _t + tt) + _p1 * (1 + 2 * _t - 2 * tt) + _p2 * (tt)) * 0.5f;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT2 EEBSpline(const FLOAT2& _p0, const FLOAT2& _p1, const FLOAT2& _p2, const FLOAT2& _p3, float _t)
	{
		float tt = _t * _t;
		float ttt = tt * _t;
		return (_p0 * (1 - 3 * _t + 3 * tt - ttt) + _p1 * (4 - 6 * tt + 3 * ttt) + _p2 * (1 + 3 * _t + 3 * tt - 3 * ttt) + _p3 * (ttt)) / 6.f;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEDDALine(const FLOAT2& _p0, const FLOAT2& _p1)
	{
		float k = abs(_p1.x - _p0.x) >= abs(_p1.y - _p0.y) ? abs(_p1.x - _p0.x) : abs(_p1.y - _p0.y);
		FLOAT2 delta((_p1.x - _p0.x) / k, (_p1.y - _p0.y) / k);
		FLOAT2 pos = _p0;
		std::vector<FLOAT2> result;

		for (float i = 0.f; i < k; ++i)
		{
			result.push_back(pos += delta);
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	void EEDDALine(const FLOAT2& _p0, const FLOAT2& _p1, std::vector<FLOAT2>& _result)
	{
		float k = abs(_p1.x - _p0.x) >= abs(_p1.y - _p0.y) ? abs(_p1.x - _p0.x) : abs(_p1.y - _p0.y);
		FLOAT2 delta((_p1.x - _p0.x) / k, (_p1.y - _p0.y) / k);
		FLOAT2 pos = _p0;

		for (float i = 0.f; i < k; ++i)
		{
			_result.push_back(pos += delta);
		}
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEBresenhamLine(const FLOAT2& _p0, const FLOAT2& _p1)
	{
		FLOAT2 delta(_p1.x - _p0.x, _p1.y - _p0.y);
		FLOAT2 forward(delta.x >= 0.f ? 1.f : -1.f, delta.y >= 0.f ? 1.f : -1.f);
		delta = FLOAT2(delta.x >= 0 ? delta.x : -delta.x, delta.y >= 0 ? delta.y : -delta.y);
		float e = 2 * delta.y - delta.x;
		FLOAT2 pos = _p0;
		std::vector<FLOAT2> result;
		while (pos.x != _p1.x)
		{
			if (e >= 0)
			{
				pos.y += forward.y;
				e -= 2 * delta.x;
			}
			else
			{
				pos.x += forward.x;
				e += 2 * delta.y;
			}
			result.push_back(pos);
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	void EEBresenhamLine(const FLOAT2& _p0, const FLOAT2& _p1, std::vector<FLOAT2>& _result)
	{
		FLOAT2 delta(_p1.x - _p0.x, _p1.y - _p0.y);
		FLOAT2 forward(delta.x >= 0.f ? 1.f : -1.f, delta.y >= 0.f ? 1.f : -1.f);
		delta = FLOAT2(delta.x >= 0 ? delta.x : -delta.x, delta.y >= 0 ? delta.y : -delta.y);
		float e = 2 * delta.y - delta.x;
		FLOAT2 pos = _p0;

		while (pos.x != _p1.x)
		{
			if (e >= 0)
			{
				pos.y += forward.y;
				e -= 2 * delta.x;
			}
			else
			{
				pos.x += forward.x;
				e += 2 * delta.y;
			}
			_result.push_back(pos);
		}
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEBresenhamArc(const FLOAT2& _pos, float _r, float _start, float _end)
	{
		float d = 3 - 2 * _r;
		FLOAT2 pos = FLOAT2(0.f, _r);
		std::vector<FLOAT2> result;

		// 45бу
		result.push_back(pos);
		while (pos.x++ <= pos.y)
		{
			if (d < 0)
			{
				result.push_back(pos);
				d += 4 * pos.x + 2;
			}
			else
			{
				--pos.y;
				result.push_back(pos);
				d += 4 * pos.x - 4 * pos.y + 2;
			}
		}
		// 90бу
		unsigned int size = result.size();
		for (unsigned int i = 0; i < size; ++i)
			result.push_back(FLOAT2(result[i].y, result[i].x));
		// 180бу
		size = result.size();
		for (unsigned int i = 0; i < size; ++i)
			result.push_back(FLOAT2(-result[i].x, result[i].y));
		// 360бу
		size = result.size();
		for (unsigned int i = 0; i < size; ++i)
			result.push_back(FLOAT2(result[i].x, -result[i].y));
		// shift
		for (FLOAT2& pos : result)
			pos += _pos;

		return result;
	}

	// f(x,y) = (x-xc)^2 + (y-yc)^2 - r^2
	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEPNArc(const FLOAT2& _pos, float _r, float _start, float _end)
	{
		float f = 0.f;
		FLOAT2 pos = FLOAT2(0.f, _r);
		std::vector<FLOAT2> result;

		// 45бу
		result.push_back(pos);
		while (pos.x <= pos.y)
		{
			if (f <= 0.f)
			{
				++pos.x;
				result.push_back(pos);
				f += 2 * pos.x - 1;
			}
			else
			{
				--pos.y;
				result.push_back(pos);
				f += -2 * pos.y - 1;
			}
		}
		// 90бу
		unsigned int size = result.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			result.push_back(FLOAT2(result[i].y, result[i].x));
		}
		// 180бу
		size = result.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			result.push_back(FLOAT2(-result[i].x, result[i].y));
		}
		// 360бу
		size = result.size();
		for (unsigned int i = 0; i < size; ++i)
		{
			result.push_back(FLOAT2(result[i].x, -result[i].y));
		}
		// shift
		for (FLOAT2& pos : result)
			pos += _pos;

		return result;
	}

	// x = a * cost, y = b * sint
	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEEllipse(const FLOAT2& _pos, float _a, float _b, float _start, float _end)
	{
		float delta = EE_PI / 360.f;
		std::vector<FLOAT2> result;

		while (_start <= _end)
		{
			result.push_back(_pos + FLOAT2(_a * cos(_start), _b * sin(_start)));
			_start += delta;
		}

		return result;
	}

	// y = kx + b
	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEFillPolygon(const std::vector<FLOAT2>& _points)
	{
		float k = tan(EE_PI / 4);
		float delta = 2.f;
		int n = _points.size();
		std::vector<FLOAT2> result;

		float bmin = EE_F_MAX, bmax = EE_F_MIN;
		for (int i = 0; i < n; i += 1)
		{
			float b = _points[i].y - k * _points[i].x;
			bmin = min(b, bmin);
			bmax = max(b, bmax);
		}

		for (float b = bmin; b < bmax; b += delta)
		{
			FLOAT2 intersection;
			std::vector<FLOAT2> intersections;
			for (int i = 0; i < n; ++i)
			{
				if (EELineIntersect(_points[i], _points[(i + 1) % n], k, b, intersection))
				{
					if (_points[i] == intersection)
						continue;
					else if (_points[(i + 1) % n] == intersection)
					{
						if (!EELineIntersect(_points[i], _points[(i + 2) % n], k, b))
							intersections.push_back(intersection);
						intersections.push_back(intersection);
						++i;
					}
					else
						intersections.push_back(intersection);
				}
			}
			std::sort(intersections.begin(), intersections.end(), [](FLOAT2 _a, FLOAT2 _b) { return _a.x < _b.x; });
			for (unsigned int i = 0; i < intersections.size(); i += 2)
			{
				EEDDALine(intersections[i], intersections[i + 1], result);
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEFillPolygon(const std::vector<FLOAT2>& _outer, const std::vector<FLOAT2>& _inner)
	{
		float k = tan(EE_PI / 4);
		float delta = 2.f;
		int n = _outer.size();
		std::vector<FLOAT2> result;

		float bmin = EE_F_MAX, bmax = EE_F_MIN;
		for (int i = 0; i < n; i += 1)
		{
			float b = _outer[i].y - k * _outer[i].x;
			bmin = min(b, bmin);
			bmax = max(b, bmax);
		}

		for (float b = bmin; b < bmax; b += delta)
		{
			// outer
			FLOAT2 intersection;
			std::vector<FLOAT2> intersections;
			for (int i = 0; i < n; ++i)
			{
				if (EELineIntersect(_outer[i], _outer[(i + 1) % n], k, b, intersection))
				{
					if (_outer[i] == intersection)
						continue;
					else if (_outer[(i + 1) % n] == intersection)
					{
						if (!EELineIntersect(_outer[i], _outer[(i + 2) % n], k, b))
							intersections.push_back(intersection);
						intersections.push_back(intersection);
						++i;
					}
					else
						intersections.push_back(intersection);
				}
			}
			// inner
			for (unsigned int i = 0; i < _inner.size(); ++i)
			{
				if (EELineIntersect(_inner[i], _inner[(i + 1) % n], k, b, intersection))
				{
					if (_inner[i] == intersection)
						continue;
					else if (_inner[(i + 1) % n] == intersection)
					{
						if (!EELineIntersect(_inner[i], _inner[(i + 2) % n], k, b))
							intersections.push_back(intersection);
						intersections.push_back(intersection);
						++i;
					}
					else
						intersections.push_back(intersection);
				}
			}
			// fill
			std::sort(intersections.begin(), intersections.end(), [](FLOAT2 _a, FLOAT2 _b) { return _a.x < _b.x; });
			for (unsigned int i = 0; i < intersections.size(); i += 2)
			{
				EEDDALine(intersections[i], intersections[i + 1], result);
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EETest()
	{
		/*
		auto mask = [=](float x, float y){
		return (y - 300.f)*(500.f - 700.f) - (x - 700.f)*(200.f - 300.f) > 0;
		};

		std::vector<FLOAT2> result;
		for (float i = 0; i < 800.f; ++i)
		for (float j = 0; j < 450.f; ++j)
		if (mask(i, j))
		result.push_back(FLOAT2(i, j));

		return result;
		*/
		std::vector<FLOAT2> result;
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	void putpolywiths(int* p, int n)
	{

	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixRotationAxis(const FLOAT3& axis, const FLOAT radians)
	{
		FLOAT3 axisN = axis.GetNormalization();

		FLOAT sinAngle = sinf(radians);
		FLOAT cosAngle = cosf(radians);
		FLOAT rmCosAngle = 1 - cosAngle;

		MATRIX result;
		result(0, 0) = (FLOAT)((axisN.x * axisN.x) * rmCosAngle + cosAngle);
		result(0, 1) = (FLOAT)((axisN.x * axisN.y) * rmCosAngle + (axisN.z * sinAngle));
		result(0, 2) = (FLOAT)((axisN.x * axisN.z) * rmCosAngle - (axisN.y * sinAngle));

		result(1, 0) = (FLOAT)((axisN.y * axisN.x) * rmCosAngle - (axisN.z * sinAngle));
		result(1, 1) = (FLOAT)((axisN.y * axisN.y) * rmCosAngle + cosAngle);
		result(1, 2) = (FLOAT)((axisN.y * axisN.z) * rmCosAngle + (axisN.x * sinAngle));

		result(2, 0) = (FLOAT)((axisN.z * axisN.x) * rmCosAngle + (axisN.y * sinAngle));
		result(2, 1) = (FLOAT)((axisN.z * axisN.y) * rmCosAngle - (axisN.x * sinAngle));
		result(2, 2) = (FLOAT)((axisN.z * axisN.z) * rmCosAngle + cosAngle);

		result(0, 3) = result(1, 3) = result(2, 3) = 0.0f;
		result(3, 0) = result(3, 1) = result(3, 2) = 0.0f;
		result(3, 3) = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixRotationAxis(const FLOAT4& axis, const FLOAT radians)
	{
		FLOAT4 axisN = axis.GetNormalization();

		FLOAT sinAngle = sinf(radians);
		FLOAT cosAngle = cosf(radians);
		FLOAT rmCosAngle = 1 - cosAngle;

		MATRIX result;
		result(0, 0) = (FLOAT)((axisN.x * axisN.x) * rmCosAngle + cosAngle);
		result(0, 1) = (FLOAT)((axisN.x * axisN.y) * rmCosAngle + (axisN.z * sinAngle));
		result(0, 2) = (FLOAT)((axisN.x * axisN.z) * rmCosAngle - (axisN.y * sinAngle));

		result(1, 0) = (FLOAT)((axisN.y * axisN.x) * rmCosAngle - (axisN.z * sinAngle));
		result(1, 1) = (FLOAT)((axisN.y * axisN.y) * rmCosAngle + cosAngle);
		result(1, 2) = (FLOAT)((axisN.y * axisN.z) * rmCosAngle + (axisN.x* sinAngle));

		result(2, 0) = (FLOAT)((axisN.z * axisN.x) * rmCosAngle + (axisN.y * sinAngle));
		result(2, 1) = (FLOAT)((axisN.z * axisN.y) * rmCosAngle - (axisN.x * sinAngle));
		result(2, 2) = (FLOAT)((axisN.z * axisN.z) * rmCosAngle + cosAngle);

		result(0, 3) = result(1, 3) = result(2, 3) = 0.0f;
		result(3, 0) = result(3, 1) = result(3, 2) = 0.0f;
		result(3, 3) = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixRotationAxisN(const FLOAT3& axisN, const FLOAT radians)
	{
		FLOAT sinAngle = sinf(radians);
		FLOAT cosAngle = cosf(radians);
		FLOAT rmCosAngle = 1 - cosAngle;

		MATRIX result;
		result(0, 0) = (FLOAT)((axisN.x * axisN.x) * rmCosAngle + cosAngle);
		result(0, 1) = (FLOAT)((axisN.x * axisN.y) * rmCosAngle + (axisN.z * sinAngle));
		result(0, 2) = (FLOAT)((axisN.x * axisN.z) * rmCosAngle - (axisN.y * sinAngle));

		result(1, 0) = (FLOAT)((axisN.y * axisN.x) * rmCosAngle - (axisN.z * sinAngle));
		result(1, 1) = (FLOAT)((axisN.y * axisN.y) * rmCosAngle + cosAngle);
		result(1, 2) = (FLOAT)((axisN.y * axisN.z) * rmCosAngle + (axisN.x* sinAngle));

		result(2, 0) = (FLOAT)((axisN.z * axisN.x) * rmCosAngle + (axisN.y * sinAngle));
		result(2, 1) = (FLOAT)((axisN.z * axisN.y) * rmCosAngle - (axisN.x * sinAngle));
		result(2, 2) = (FLOAT)((axisN.z * axisN.z) * rmCosAngle + cosAngle);

		result(0, 3) = result(1, 3) = result(2, 3) = 0.0f;
		result(3, 0) = result(3, 1) = result(3, 2) = 0.0f;
		result(3, 3) = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixRotationAxisN(const FLOAT4& axisN, const FLOAT radians)
	{
		FLOAT sinAngle = sinf(radians);
		FLOAT cosAngle = cosf(radians);
		FLOAT rmCosAngle = 1 - cosAngle;

		MATRIX result;
		result(0, 0) = (FLOAT)((axisN.x * axisN.x) * rmCosAngle + cosAngle);
		result(0, 1) = (FLOAT)((axisN.x * axisN.y) * rmCosAngle + (axisN.z * sinAngle));
		result(0, 2) = (FLOAT)((axisN.x * axisN.z) * rmCosAngle - (axisN.y * sinAngle));

		result(1, 0) = (FLOAT)((axisN.y * axisN.x) * rmCosAngle - (axisN.z * sinAngle));
		result(1, 1) = (FLOAT)((axisN.y * axisN.y) * rmCosAngle + cosAngle);
		result(1, 2) = (FLOAT)((axisN.y * axisN.z) * rmCosAngle + (axisN.x* sinAngle));

		result(2, 0) = (FLOAT)((axisN.z * axisN.x) * rmCosAngle + (axisN.y * sinAngle));
		result(2, 1) = (FLOAT)((axisN.z * axisN.y) * rmCosAngle - (axisN.x * sinAngle));
		result(2, 2) = (FLOAT)((axisN.z * axisN.z) * rmCosAngle + cosAngle);

		result(0, 3) = result(1, 3) = result(2, 3) =
			result(3, 0) = result(3, 1) = result(3, 2) = 0.0f;
		result(3, 3) = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	//the matrix is for the special coordinate system of the 2-D screen only
	MATRIX MatrixOrthoLH(FLOAT _Width, FLOAT _Height, FLOAT _nearZ, FLOAT _farZ)
	{
		MATRIX result;

		FLOAT range = 1.0f / (_farZ - _nearZ);
		result(0, 0) = 2.0f / _Width;	result(0, 1) = 0.0f;				result(0, 2) = 0.0f;			result(0, 3) = 0;
		result(1, 0) = 0.0f;			result(1, 1) = -2.0f / _Height;	result(1, 2) = 0.0f;			result(1, 3) = 0;
		result(2, 0) = 0.0f;			result(2, 1) = 0.0f;				result(2, 2) = range;			result(2, 3) = 0;
		result(3, 0) = -1.0f;			result(3, 1) = 1.0f;				result(3, 2) = -range * _nearZ;	result(3, 3) = 1;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixPerspectiveFovLH(const FLOAT fovY, const FLOAT aspectRatio, const FLOAT nearZ, const FLOAT farZ)
	{
		MATRIX result;

		result.m[0][0] = 1 / (tan(fovY * 0.5f) *aspectRatio);
		result.m[1][1] = 1 / tan(fovY * 0.5f);
		result.m[2][2] = farZ / (farZ - nearZ);
		result.m[2][3] = 1.0f;
		result.m[3][2] = (nearZ * farZ) / (nearZ - farZ);

		/*
		result.m[0][1] = result.m[0][2] = result.m[0][3] =
		result.m[1][0] = result.m[1][2] = result.m[1][3] =
		result.m[2][0] = result.m[2][1] =
		result.m[3][0] = result.m[3][1] = result.m[3][3] = 0.f;
		*/

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixScaling(const FLOAT3& _scale)
	{
		MATRIX result;

		result.m[0][0] = _scale.x;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = _scale.y;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = _scale.z;
		result.m[2][3] = 0.0f;

		result.m[3][0] = 0.f;
		result.m[3][1] = 0.f;
		result.m[3][2] = 0.f;
		result.m[3][3] = 1.f;

		return result;	
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixTranslation(const FLOAT3& _pos)
	{
		MATRIX result;

		result.m[0][0] = 1.0f;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = 1.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = 1.0f;
		result.m[2][3] = 0.0f;

		result.m[3][0] = _pos.x;
		result.m[3][1] = _pos.y;
		result.m[3][2] = _pos.z;
		result.m[3][3] = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixTranslation(const FLOAT _x, const FLOAT _y, const FLOAT _z)
	{
		MATRIX result;

		result.m[0][0] = 1.0f;
		result.m[0][1] = 0.0f;
		result.m[0][2] = 0.0f;
		result.m[0][3] = 0.0f;

		result.m[1][0] = 0.0f;
		result.m[1][1] = 1.0f;
		result.m[1][2] = 0.0f;
		result.m[1][3] = 0.0f;

		result.m[2][0] = 0.0f;
		result.m[2][1] = 0.0f;
		result.m[2][2] = 1.0f;
		result.m[2][3] = 0.0f;

		result.m[3][0] = _x;
		result.m[3][1] = _y;
		result.m[3][2] = _z;
		result.m[3][3] = 1.0f;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT MatrixMinorDeterminant(
		const MATRIX& _matrix,
		const UINT _r0, const UINT _r1, const UINT _r2,
		const UINT _c0, const UINT _c1, const UINT _c2)
	{
		return _matrix.m[_r0][_c0] * (_matrix.m[_r1][_c1] * _matrix.m[_r2][_c2] - _matrix.m[_r2][_c1] * _matrix.m[_r1][_c2]) -
			_matrix.m[_r0][_c1] * (_matrix.m[_r1][_c0] * _matrix.m[_r2][_c2] - _matrix.m[_r2][_c0] * _matrix.m[_r1][_c2]) +
			_matrix.m[_r0][_c2] * (_matrix.m[_r1][_c0] * _matrix.m[_r2][_c1] - _matrix.m[_r2][_c0] * _matrix.m[_r1][_c1]);
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT MatrixDeterminant(const MATRIX& _matrix)
	{
		return _matrix.m[0][0] * MatrixMinorDeterminant(_matrix, 1, 2, 3, 1, 2, 3) -
			_matrix.m[0][1] * MatrixMinorDeterminant(_matrix, 1, 2, 3, 0, 2, 3) +
			_matrix.m[0][2] * MatrixMinorDeterminant(_matrix, 1, 2, 3, 0, 1, 3) -
			_matrix.m[0][3] * MatrixMinorDeterminant(_matrix, 1, 2, 3, 0, 1, 2);
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixInverse(const MATRIX& _matrix)
	{
		FLOAT m00 = _matrix.m[0][0], m01 = _matrix.m[0][1], m02 = _matrix.m[0][2], m03 = _matrix.m[0][3];
		FLOAT m10 = _matrix.m[1][0], m11 = _matrix.m[1][1], m12 = _matrix.m[1][2], m13 = _matrix.m[1][3];
		FLOAT m20 = _matrix.m[2][0], m21 = _matrix.m[2][1], m22 = _matrix.m[2][2], m23 = _matrix.m[2][3];
		FLOAT m30 = _matrix.m[3][0], m31 = _matrix.m[3][1], m32 = _matrix.m[3][2], m33 = _matrix.m[3][3];

		FLOAT v0 = m20 * m31 - m21 * m30;
		FLOAT v1 = m20 * m32 - m22 * m30;
		FLOAT v2 = m20 * m33 - m23 * m30;
		FLOAT v3 = m21 * m32 - m22 * m31;
		FLOAT v4 = m21 * m33 - m23 * m31;
		FLOAT v5 = m22 * m33 - m23 * m32;

		FLOAT t00 = +(v5 * m11 - v4 * m12 + v3 * m13);
		FLOAT t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		FLOAT t20 = +(v4 * m10 - v2 * m11 + v0 * m13);
		FLOAT t30 = -(v3 * m10 - v1 * m11 + v0 * m12);

		FLOAT invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

		FLOAT d00 = t00 * invDet;
		FLOAT d10 = t10 * invDet;
		FLOAT d20 = t20 * invDet;
		FLOAT d30 = t30 * invDet;

		FLOAT d01 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		FLOAT d11 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		FLOAT d21 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		FLOAT d31 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;

		FLOAT d02 = +(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		FLOAT d12 = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		FLOAT d22 = +(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		FLOAT d32 = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;

		FLOAT d03 = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		FLOAT d13 = +(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		FLOAT d23 = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		FLOAT d33 = +(v3 * m00 - v1 * m01 + v0 * m02) * invDet;

		return MATRIX(
			d00, d01, d02, d03,
			d10, d11, d12, d13,
			d20, d21, d22, d23,
			d30, d31, d32, d33);
	}

	//----------------------------------------------------------------------------------------------------
	Quaternion QuaternionRotationAxis(const FLOAT3& _axis, const FLOAT _radians)
	{
		Quaternion result;
		FLOAT3 axis = _axis.GetNormalization();
		FLOAT ccc = cos(0.5f * _radians);
		FLOAT sss = sin(0.5f * _radians);

		result.t = ccc;
		result.x = sss * axis.x;
		result.y = sss * axis.y;
		result.z = sss * axis.z;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 operator* (const FLOAT3 float3, const MATRIX matrix)
	{
		FLOAT3 result;
		result.x = float3.x * matrix(0, 0) + float3.y * matrix(1, 0) + float3.z * matrix(2, 0);
		result.y = float3.x * matrix(0, 1) + float3.y * matrix(1, 1) + float3.z * matrix(2, 1);
		result.z = float3.x * matrix(0, 2) + float3.y * matrix(1, 2) + float3.z * matrix(2, 2);

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT4 operator* (const FLOAT4 float4, const MATRIX matrix)
	{
		FLOAT4 result;
		result.x = float4.x * matrix(0, 0) + float4.y * matrix(1, 0) + float4.z * matrix(2, 0);
		result.y = float4.x * matrix(0, 1) + float4.y * matrix(1, 1) + float4.z * matrix(2, 1);
		result.z = float4.x * matrix(0, 2) + float4.y * matrix(1, 2) + float4.z * matrix(2, 2);
		result.w = float4.w;

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3& operator*= (FLOAT3& float3, const MATRIX matrix)
	{
		FLOAT3 result;
		result.x = float3.x * matrix(0, 0) + float3.y * matrix(1, 0) + float3.z * matrix(2, 0);
		result.y = float3.x * matrix(0, 1) + float3.y * matrix(1, 1) + float3.z * matrix(2, 1);
		result.z = float3.x * matrix(0, 2) + float3.y * matrix(1, 2) + float3.z * matrix(2, 2);
		float3 = result;

		return float3;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT4& operator*= (FLOAT4& float4, const MATRIX matrix)
	{
		FLOAT4 result;
		result.x = float4.x * matrix(0, 0) + float4.y * matrix(1, 0) + float4.z * matrix(2, 0);
		result.y = float4.x * matrix(0, 1) + float4.y * matrix(1, 1) + float4.z * matrix(2, 1);
		result.z = float4.x * matrix(0, 2) + float4.y * matrix(1, 2) + float4.z * matrix(2, 2);
		result.w = float4.w;
		float4 = result;

		return float4;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 Rand_FLOAT3()
	{
		return FLOAT3((float)(rand() / double(RAND_MAX)), (float)(rand() / double(RAND_MAX)), (float)(rand() / double(RAND_MAX)));
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT4 Rand_FLOAT4()
	{
		return FLOAT4((float)(rand() / double(RAND_MAX)), (float)(rand() / double(RAND_MAX)), (float)(rand() / double(RAND_MAX)), (float)(rand() / double(RAND_MAX)));
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT EEDegreesToRadians(FLOAT _degrees)
	{
		return _degrees * (EE_PI / 180.0f);
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineIntersect(const FLOAT2& _pos0, const FLOAT2& _pos1, float _k, float _b)
	{
		FLOAT2 intersection;
		if (_pos1.x == _pos0.x)
		{
			intersection.x = _pos1.x;
			intersection.y = _k * intersection.x + _b;
		}
		else
		{
			float k = (_pos1.y - _pos0.y) / (_pos1.x - _pos0.x);
			float b = _pos0.y - k * _pos0.x;
			// parallel
			if (k - _k == 0.f)
				return false;

			intersection.x = (b - _b) / (_k - k);
			intersection.y = _k * intersection.x + _b;
		}
		if (_pos0.x < _pos1.x)
		{
			if (intersection.x < _pos0.x || _pos1.x < intersection.x)
				return false;
		}
		else if (intersection.x < _pos1.x || _pos0.x < intersection.x)
			return false;
		if (_pos0.y < _pos1.y)
		{
			if (intersection.y < _pos0.y || _pos1.y < intersection.y)
				return false;
		}
		else if (intersection.y < _pos1.y || _pos0.y < intersection.y)
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineIntersect(const FLOAT2& _pos0, const FLOAT2& _pos1, float _k, float _b, FLOAT2& _result)
	{
		if (_pos1.x == _pos0.x)
		{
			_result.x = _pos1.x;
			_result.y = _k * _result.x + _b;
		}
		else
		{
			float k = (_pos1.y - _pos0.y) / (_pos1.x - _pos0.x);
			float b = _pos0.y - k * _pos0.x;
			// parallel
			if (k - _k == 0.f)
				return false;

			_result.x = (b - _b) / (_k - k);
			_result.y = _k * _result.x + _b;
		}
		if (_pos0.x < _pos1.x)
		{
			if (_result.x < _pos0.x || _pos1.x < _result.x)
				return false;
		}
		else if (_result.x < _pos1.x || _pos0.x < _result.x)
			return false;
		if (_pos0.y < _pos1.y)
		{
			if (_result.y < _pos0.y || _pos1.y < _result.y)
				return false;
		}
		else if (_result.y < _pos1.y || _pos0.y < _result.y)
			return false;

		return true;
	}
}