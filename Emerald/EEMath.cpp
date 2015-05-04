#include "EEMath.h"

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
	std::vector<FLOAT2> EEBresenhamLine(const FLOAT2& _p0, const FLOAT2& _p1)
	{
		FLOAT2 delta(_p1.x - _p0.x, _p1.y - _p0.y);
		float e = 2 * delta.y - delta.x;
		FLOAT2 pos = _p0;
		std::vector<FLOAT2> result;
		while (pos.x != _p1.x)
		{
			if (e >= 0)
			{
				++pos.y;
				e -= 2 * delta.x;
			}
			else
			{
				++pos.x;
				e += 2 * delta.y;
			}
			result.push_back(pos);
		}

		return result;
	}

	// 
	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEBresenhamArc(const FLOAT2& _pos, const float _r)
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

	// f(x,y) = (x-xc)^2 + (y-yc)^2 - r^2
	//----------------------------------------------------------------------------------------------------
	std::vector<FLOAT2> EEPNArc(const FLOAT2& _pos, const float _r)
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
	void putpolywiths(int* p, int n) {
		float alpha = EE_PI / 4, k = tan(alpha), db = 4;

		// get range of b
		// TODO: here magic
		int bmin = 1000, bmax = -1;
		auto getb = [](int x, int y, int k) {
			return y - k*x;
		};
		for (int i = 0; i < n; i += 1) {
			int b = getb(p[i * 2], p[i * 2 + 1], k);
			bmin = min(b, bmin);
			bmax = max(b, bmax);
		}
		struct Line{ int k, b; };
		struct Point{ int x, y; };
		struct Line2{ Point a, b; };
		auto getintersect = [](int x0, int y0, int x1, int y1, int b, float k, int& ix, int& iy) {

			if (abs(k*x0 + y1 - k*x1 + y0) <= 0.01) return 0;
			// k f
			float t = k*(x0 - x1) - y0 + y1;
			ix = (b*(x1 - x0) + x0*y1 - x1*y0) / t;
			iy = ((-b*y0) + b*y1 + k*x0*y1 - k*x1*y0) / t;
			if (x0>x1) std::swap(x0, x1);
			if (x0 > ix || ix > x1) return 0; // box detection
			return 1;
		};

		for (int b = bmin; b < bmax; b += db) {
			int x0, y0, x1, y1;
			int i;
			for (i = 0; i < n; i += 1) {
				int t = i * 2;
				if (getintersect(p[i * 2], p[i * 2 + 1], p[(t + 2) % (2 * n)], p[(t + 3) % (2 * n)], b, k, x0, y0)) break;
			}
			for (++i; i < n; i += 1) {
				int t = i * 2;
				if (getintersect(p[i * 2], p[i * 2 + 1], p[(t + 2) % (2 * n)], p[(t + 3) % (2 * n)], b, k, x1, y1)) break;
			}
			//EEPoints2D points(EEBresenhamLine(FLOAT2(x0, y0), FLOAT2(x1, y1), c);
		}
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
		result(1, 0) = 0.0f;			result(1, 1) = - 2.0f / _Height;	result(1, 2) = 0.0f;			result(1, 3) = 0;
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
}