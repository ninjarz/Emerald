#pragma once
#ifndef _EE_MATH_H_
#define _EE_MATH_H_

#include <Windows.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
//----------------------------------------------------------------------------------------------------
#define EE_PI               3.141592654f
#define EE_2PI              6.283185307f
#define EE_1DIVPI           0.318309886f
#define EE_1DIV2PI          0.159154943f
#define EE_PIDIV2           1.570796327f
#define EE_PIDIV4           0.785398163f

	//----------------------------------------------------------------------------------------------------
	//typedef unsigned int UINT;

	//----------------------------------------------------------------------------------------------------
	typedef float FLOAT;

	//----------------------------------------------------------------------------------------------------
	struct INT2
	{
		int x;
		int y;

		inline INT2() : x(0), y(0)
		{
		}

		inline INT2(const INT _x, const INT _y) : x(_x), y(_y)
		{
		}

		inline explicit INT2(const INT _iArray[2]) : x(_iArray[0]), y(_iArray[1])
		{
		}

		inline explicit INT2(const INT _scaler) : x(_scaler), y(_scaler)
		{
		}

		inline INT operator[] (const UINT i) const
		{
			if (i < 2)
				return *(&x + i);
			return y;
		}

		inline INT& operator[] (const UINT i)
		{
			if (i < 2)
				return *(&x + i);
			return y;
		}

		inline INT2& operator= (const INT2& _int2)
		{
			x = _int2.x;
			y = _int2.y;
			return *this;
		}

		inline INT2& operator= (const INT _scalar)
		{
			x = _scalar;
			y = _scalar;
			return *this;
		}

		inline INT2 operator+ (const INT2& _int2) const
		{
			return INT2(x + _int2.x, y + _int2.y);
		}

		inline INT2 operator- (const INT2& _int2) const
		{
			return INT2(x - _int2.x, y - _int2.y);
		}

		inline INT2 operator* (const INT _scalar) const
		{
			return INT2(x * _scalar, y * _scalar);
		}

		inline INT2 operator* (const INT2& _int2) const
		{
			return INT2(x * _int2.x, y * _int2.y);
		}

		inline INT2 operator/ (const INT _scalar) const
		{
			float inv = 0.f;
			if (_scalar != 0)
			{
				inv = 1.0f / _scalar;
			}
			return INT2(int(x * inv), int(y * inv));
		}

		inline INT2 operator/ (const INT2& _int2) const
		{
			return INT2(x / _int2.x, y / _int2.y);
		}

		inline const INT2& operator+ () const
		{
			return *this;
		}

		inline INT2 operator- () const
		{
			return INT2(-x, -y);
		}

		inline friend INT2 operator* (const INT _scalar, const INT2& int2)
		{
			return INT2(_scalar * int2.x, _scalar * int2.y);
		}

		inline friend INT2 operator/ (const INT _scalar, const INT2& int2)
		{
			return INT2(_scalar / int2.x, _scalar / int2.y);
		}

		inline INT2& operator+= (const INT2& _int2)
		{
			x += _int2.x;
			y += _int2.y;
			return *this;
		}

		inline INT2& operator+= (const INT _scaler)
		{
			x += _scaler;
			y += _scaler;
			return *this;
		}

		inline INT2& operator-= (const INT2& _int2)
		{
			x -= _int2.x;
			y -= _int2.y;
			return *this;
		}

		inline INT2& operator-= (const INT _scaler)
		{
			x -= _scaler;
			y -= _scaler;
			return *this;
		}

		inline INT2& operator*= (const INT _scalar)
		{
			x *= _scalar;
			y *= _scalar;
			return *this;
		}

		inline INT2& operator*= (const INT2& _int2)
		{
			x *= _int2.x;
			y *= _int2.y;
			return *this;
		}
	};

	//----------------------------------------------------------------------------------------------------
	struct FLOAT2
	{
		float x;
		float y;

		inline FLOAT2() : x(0.0f), y(0.0f)
		{
		}

		inline FLOAT2(const FLOAT _x, const FLOAT _y) : x(_x), y(_y)
		{
		}

		inline explicit FLOAT2(const FLOAT _fArray[2]) : x(_fArray[0]), y(_fArray[1])
		{
		}

		inline explicit FLOAT2(const FLOAT _scaler) : x(_scaler), y(_scaler)
		{
		}

		inline FLOAT operator[] (const UINT i) const
		{
			if (i < 2)
				return *(&x + i);
			return y;
		}

		inline FLOAT& operator[] (const UINT i)
		{
			if (i < 2)
				return *(&x + i);
			return y;
		}

		inline FLOAT2& operator= (const FLOAT2& _float2)
		{
			x = _float2.x;
			y = _float2.y;
			return *this;
		}

		inline FLOAT2& operator= (const FLOAT _scalar)
		{
			x = _scalar;
			y = _scalar;
			return *this;
		}

		inline FLOAT2 operator+ (const FLOAT2& _float2) const
		{
			return FLOAT2(x + _float2.x, y + _float2.y);
		}

		inline FLOAT2 operator- (const FLOAT2& _float2) const
		{
			return FLOAT2(x - _float2.x, y - _float2.y);
		}

		inline FLOAT2 operator* (const FLOAT _scalar) const
		{
			return FLOAT2(x * _scalar, y * _scalar);
		}

		inline FLOAT2 operator* (const FLOAT2& _float2) const
		{
			return FLOAT2(x * _float2.x, y * _float2.y);
		}

		inline FLOAT2 operator/ (const FLOAT _scalar) const
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			return FLOAT2(x * inv, y * inv);
		}

		inline FLOAT2 operator/ (const FLOAT2& _float2) const
		{
			return FLOAT2(x / _float2.x, y / _float2.y);
		}

		inline const FLOAT2& operator+ () const
		{
			return *this;
		}

		inline FLOAT2 operator- () const
		{
			return FLOAT2(-x, -y);
		}

		inline friend FLOAT2 operator* (const float _scalar, const FLOAT2& float2)
		{
			return FLOAT2(_scalar * float2.x, _scalar * float2.y);
		}

		inline friend FLOAT2 operator/ (const float _scalar, const FLOAT2& float2)
		{
			return FLOAT2(_scalar / float2.x, _scalar / float2.y);
		}

		inline FLOAT2& operator+= (const FLOAT2& _float2)
		{
			x += _float2.x;
			y += _float2.y;
			return *this;
		}

		inline FLOAT2& operator+= (const FLOAT _scaler)
		{
			x += _scaler;
			y += _scaler;
			return *this;
		}

		inline FLOAT2& operator-= (const FLOAT2& _float2)
		{
			x -= _float2.x;
			y -= _float2.y;
			return *this;
		}

		inline FLOAT2& operator-= (const FLOAT _scaler)
		{
			x -= _scaler;
			y -= _scaler;
			return *this;
		}

		inline FLOAT2& operator*= (const FLOAT _scalar)
		{
			x *= _scalar;
			y *= _scalar;
			return *this;
		}

		inline FLOAT2& operator*= (const FLOAT2& _float2)
		{
			x *= _float2.x;
			y *= _float2.y;
			return *this;
		}

		inline FLOAT2& operator/= (const FLOAT _scalar)
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			x *= inv;
			y *= inv;
			return *this;
		}

		inline FLOAT2& operator/= (const FLOAT2& _float2)
		{
			x /= _float2.x;
			y /= _float2.y;
			return *this;
		}

		inline FLOAT Length() const
		{
			return sqrt(x * x + y * y);
		}

		inline FLOAT Distance(const FLOAT2& _float2) const
		{
			return (*this - _float2).Length();
		}

		inline FLOAT DotProduct(const FLOAT2& _float2) const
		{
			return x * _float2.x + y * _float2.y;
		}

		inline FLOAT CrossProduct(const FLOAT2& _float2) const
		{
			return x * _float2.y - y * _float2.x;
		}

		inline FLOAT Normalise(void)
		{
			FLOAT length = sqrt(x * x + y * y);
			if (length > 0.0f)
			{
				FLOAT invLength = 1.0f / length;
				x *= invLength;
				y *= invLength;
			}
			return length;
		}

		inline FLOAT2 GetNormalization(void) const
		{
			FLOAT2 result = *this;
			result.Normalise();
			return result;
		}
	};

	//----------------------------------------------------------------------------------------------------
	struct FLOAT3
	{
		float x;
		float y;
		float z;

		inline FLOAT3() : x(0.0f), y(0.0f), z(0.0f)
		{
		}

		inline FLOAT3(const FLOAT _x, const FLOAT _y, const FLOAT _z) : x(_x), y(_y), z(_z)
		{
		}

		inline explicit FLOAT3(const FLOAT _fArray[3]) : x(_fArray[0]), y(_fArray[1]), z(_fArray[2])
		{
		}

		inline explicit FLOAT3(const FLOAT _scaler) : x(_scaler), y(_scaler), z(_scaler)
		{
		}

		inline FLOAT operator[] (const UINT i) const
		{
			if (i < 3)
				return *(&x + i);
			return z;
		}

		inline FLOAT& operator[] (const UINT i)
		{
			if (i < 3)
				return *(&x + i);
			return z;
		}

		inline FLOAT3& operator= (const FLOAT3& _float3)
		{
			x = _float3.x;
			y = _float3.y;
			z = _float3.z;
			return *this;
		}

		inline FLOAT3& operator= (const FLOAT _scalar)
		{
			x = _scalar;
			y = _scalar;
			z = _scalar;
			return *this;
		}

		inline FLOAT3 operator+ (const FLOAT& _float) const
		{
			return FLOAT3(x + _float, y + _float, z + _float);
		}

		inline FLOAT3 operator+ (const FLOAT2& _float2) const
		{
			return FLOAT3(x + _float2.x, y + _float2.y, z);
		}

		inline FLOAT3 operator+ (const FLOAT3& _float3) const
		{
			return FLOAT3(x + _float3.x, y + _float3.y, z + _float3.z);
		}

		inline FLOAT3 operator- (const FLOAT& _float) const
		{
			return FLOAT3(x - _float, y - _float, z - _float);
		}

		inline FLOAT3 operator- (const FLOAT2& _float2) const
		{
			return FLOAT3(x - _float2.x, y - _float2.y, z);
		}

		inline FLOAT3 operator- (const FLOAT3& _float3) const
		{
			return FLOAT3(x - _float3.x, y - _float3.y, z - _float3.z);
		}

		inline FLOAT3 operator* (const FLOAT _scalar) const
		{
			return FLOAT3(x * _scalar, y * _scalar, z * _scalar);
		}

		inline FLOAT3 operator* (const FLOAT3& _float3) const
		{
			return FLOAT3(x * _float3.x, y * _float3.y, z * _float3.z);
		}

		inline FLOAT3 operator/ (const FLOAT _scalar) const
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			return FLOAT3(x * inv, y * inv, z * inv);
		}

		inline FLOAT3 operator/ (const FLOAT3& _float3) const
		{
			return FLOAT3(x / _float3.x, y / _float3.y, z / _float3.z);
		}

		inline const FLOAT3& operator+ () const
		{
			return *this;
		}

		inline FLOAT3 operator- () const
		{
			return FLOAT3(-x, -y, -z);
		}

		inline friend FLOAT3 operator* (const float _scalar, const FLOAT3& float3)
		{
			return FLOAT3(_scalar * float3.x, _scalar * float3.y, _scalar * float3.z);
		}

		inline friend FLOAT3 operator/ (const float _scalar, const FLOAT3& float3)
		{
			return FLOAT3(_scalar / float3.x, _scalar / float3.y, _scalar / float3.z);
		}

		inline FLOAT3& operator+= (const FLOAT3& _float3)
		{
			x += _float3.x;
			y += _float3.y;
			z += _float3.z;
			return *this;
		}

		inline FLOAT3& operator+= (const FLOAT _scaler)
		{
			x += _scaler;
			y += _scaler;
			z += _scaler;
			return *this;
		}

		inline FLOAT3& operator-= (const FLOAT3& _float3)
		{
			x -= _float3.x;
			y -= _float3.y;
			z -= _float3.z;
			return *this;
		}

		inline FLOAT3& operator-= (const FLOAT _scaler)
		{
			x -= _scaler;
			y -= _scaler;
			z -= _scaler;
			return *this;
		}

		inline FLOAT3& operator*= (const FLOAT _scalar)
		{
			x *= _scalar;
			y *= _scalar;
			z *= _scalar;
			return *this;
		}

		inline FLOAT3& operator*= (const FLOAT3& _float3)
		{
			x *= _float3.x;
			y *= _float3.y;
			z *= _float3.z;
			return *this;
		}

		inline FLOAT3& operator/= (const FLOAT _scalar)
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}

		inline FLOAT3& operator/= (const FLOAT3& _float3)
		{
			x /= _float3.x;
			y /= _float3.y;
			z /= _float3.z;
			return *this;
		}

		inline FLOAT Length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline FLOAT Distance(const FLOAT3& _float3) const
		{
			return (*this - _float3).Length();
		}

		inline FLOAT DotProduct(const FLOAT3& _float3) const
		{
			return x * _float3.x + y * _float3.y + z * _float3.z;
		}

		inline FLOAT3 CrossProduct(const FLOAT3& _float3) const
		{
			return FLOAT3(
				y * _float3.z - z * _float3.y,
				z * _float3.x - x * _float3.z,
				x * _float3.y - y * _float3.x);
		}

		inline FLOAT Normalise(void)
		{
			FLOAT length = sqrt(x * x + y * y + z * z);
			if (length > 0.0f)
			{
				FLOAT invLength = 1.0f / length;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			return length;
		}

		inline FLOAT3 GetNormalization(void) const
		{
			FLOAT3 result = *this;
			result.Normalise();
			return result;
		}
	};

	//----------------------------------------------------------------------------------------------------
	struct FLOAT4
	{
		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
			float f[4];
		};

		inline FLOAT4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{
		}

		inline FLOAT4(const FLOAT _x, const FLOAT _y, const FLOAT _z, const FLOAT _w) : x(_x), y(_y), z(_z), w(_w)
		{
		}

		inline explicit FLOAT4(const FLOAT _fArray[4]) : x(_fArray[0]), y(_fArray[1]), z(_fArray[2]), w(_fArray[3])
		{
		}

		inline explicit FLOAT4(const FLOAT _scaler) : x(_scaler), y(_scaler), z(_scaler), w(_scaler)
		{
		}

		inline FLOAT operator[] (const UINT i) const
		{
			if (i < 4)
				return *(&x + i);
			return z;
		}

		inline FLOAT& operator[] (const UINT i)
		{
			if (i < 4)
				return *(&x + i);
			return z;
		}

		inline FLOAT4& operator= (const FLOAT4& _float4)
		{
			x = _float4.x;
			y = _float4.y;
			z = _float4.z;
			w = _float4.w;
			return *this;
		}

		inline FLOAT4& operator= (const FLOAT _scalar)
		{
			x = _scalar;
			y = _scalar;
			z = _scalar;
			w = _scalar;
			return *this;
		}

		inline FLOAT4 operator+ (const FLOAT4& _float4) const
		{
			return FLOAT4(x + _float4.x, y + _float4.y, z + _float4.z, w + _float4.w);
		}

		inline FLOAT4 operator- (const FLOAT4& _float4) const
		{
			return FLOAT4(x - _float4.x, y - _float4.y, z - _float4.z, w - _float4.w);
		}

		inline FLOAT4 operator* (const FLOAT _scalar) const
		{
			return FLOAT4(x * _scalar, y * _scalar, z * _scalar, w * _scalar);
		}

		inline FLOAT4 operator* (const FLOAT4& _float4) const
		{
			return FLOAT4(x * _float4.x, y * _float4.y, z * _float4.z, w * _float4.w);
		}

		inline FLOAT4 operator/ (const FLOAT _scalar) const
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			return FLOAT4(x * inv, y * inv, z * inv, w * inv);
		}

		inline FLOAT4 operator/ (const FLOAT4& _float4) const
		{
			return FLOAT4(x / _float4.x, y / _float4.y, z / _float4.z, w / _float4.w);
		}

		inline const FLOAT4& operator+ () const
		{
			return *this;
		}

		inline FLOAT4 operator- () const
		{
			return FLOAT4(-x, -y, -z, -w);
		}

		inline friend FLOAT4 operator* (const float _scalar, const FLOAT4& float4)
		{
			return FLOAT4(_scalar * float4.x, _scalar * float4.y, _scalar * float4.z, _scalar * float4.w);
		}

		inline friend FLOAT4 operator/ (const float _scalar, const FLOAT4& float4)
		{
			return FLOAT4(_scalar / float4.x, _scalar / float4.y, _scalar / float4.z, _scalar / float4.w);
		}

		inline FLOAT4& operator+= (const FLOAT4& _float4)
		{
			x += _float4.x;
			y += _float4.y;
			z += _float4.z;
			w += _float4.w;
			return *this;
		}

		inline FLOAT4& operator+= (const FLOAT _scaler)
		{
			x += _scaler;
			y += _scaler;
			z += _scaler;
			w += _scaler;
			return *this;
		}

		inline FLOAT4& operator-= (const FLOAT4& _float4)
		{
			x -= _float4.x;
			y -= _float4.y;
			z -= _float4.z;
			w -= _float4.w;
			return *this;
		}

		inline FLOAT4& operator-= (const FLOAT _scaler)
		{
			x -= _scaler;
			y -= _scaler;
			z -= _scaler;
			w -= _scaler;
			return *this;
		}

		inline FLOAT4& operator*= (const FLOAT _scalar)
		{
			x *= _scalar;
			y *= _scalar;
			z *= _scalar;
			w *= _scalar;
			return *this;
		}

		inline FLOAT4& operator*= (const FLOAT4& _float4)
		{
			x *= _float4.x;
			y *= _float4.y;
			z *= _float4.z;
			w *= _float4.w;
			return *this;
		}

		inline FLOAT4& operator/= (const FLOAT _scalar)
		{
			FLOAT inv = 0.0f;
			if (_scalar != 0.0f)
			{
				inv = 1.0f / _scalar;
			}
			x *= inv;
			y *= inv;
			z *= inv;
			w *= inv;
			return *this;
		}

		inline FLOAT4& operator/= (const FLOAT4& _float4)
		{
			x /= _float4.x;
			y /= _float4.y;
			z /= _float4.z;
			w /= _float4.w;
			return *this;
		}

		inline FLOAT Length() const
		{
			return sqrt(x * x + y * y + z * z + w * w);
		}

		inline FLOAT Distance(const FLOAT4& _float4) const
		{
			return (*this - _float4).Length();
		}

		inline FLOAT DotProduct(const FLOAT4& _float4) const
		{
			return x * _float4.x + y * _float4.y + z * _float4.z + w * _float4.w;
		}

		//inline FLOAT4 crossProduct(const FLOAT4& _float4) const
		//{
		//}

		inline FLOAT Normalise(void)
		{
			FLOAT length = sqrt(x * x + y * y + z * z + w * w);
			if (length > 0.0f)
			{
				FLOAT invLength = 1.0f / length;
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w *= invLength;
			}
			return length;
		}

		inline FLOAT4 GetNormalization(void) const
		{
			FLOAT4 result = *this;
			result.Normalise();
			return result;
		};
	};

	//----------------------------------------------------------------------------------------------------
	struct VECTOR
	{
	float x;
	float y;
	float z;
	float w;
	};

	//----------------------------------------------------------------------------------------------------
	struct MATRIX
	{
		union
		{
			struct
			{
				FLOAT m11, m12, m13, m14;
				FLOAT m21, m22, m23, m24;
				FLOAT m31, m32, m33, m34;
				FLOAT m41, m42, m43, m44;
			};
			FLOAT  m[4][4];
		};

		inline MATRIX()
		{
			m11 = m22 = m33 = m44 = 1.0f;
			m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 = 0.0f;
		}

		inline MATRIX
			(
			float _m11, float _m12, float _m13, float _m14,
			float _matrix1, float _matrix2, float _matrix3, float _matrix4,
			float _m31, float _m32, float _m33, float _m34,
			float _m41, float _m42, float _m43, float _m44
			)
		{
			m11 = _m11; m12 = _m12; m13 = _m13; m14 = _m14;
			m21 = _matrix1; m22 = _matrix2; m23 = _matrix3; m24 = _matrix4;
			m31 = _m31; m32 = _m32; m33 = _m33; m34 = _m34;
			m41 = _m41; m42 = _m42; m43 = _m43; m44 = _m44;
		}

		inline MATRIX(const FLOAT* _fArray)
		{
			m11 = _fArray[0];  m12 = _fArray[1];  m13 = _fArray[2];  m14 = _fArray[3];
			m21 = _fArray[4];  m22 = _fArray[5];  m23 = _fArray[6];  m24 = _fArray[7];
			m31 = _fArray[8];  m32 = _fArray[9];  m33 = _fArray[10]; m34 = _fArray[11];
			m41 = _fArray[12]; m42 = _fArray[13]; m43 = _fArray[14]; m44 = _fArray[15];
		}

		inline FLOAT operator() (UINT _row, UINT _column) const
		{
			return m[_row][_column];
		}

		inline FLOAT& operator() (UINT _row, UINT _column)
		{
			return m[_row][_column];
		}

		inline MATRIX operator+ (const MATRIX& _matrix) const
		{
			MATRIX result;

			result.m[0][0] = m[0][0] + _matrix.m[0][0];
			result.m[0][1] = m[0][1] + _matrix.m[0][1];
			result.m[0][2] = m[0][2] + _matrix.m[0][2];
			result.m[0][3] = m[0][3] + _matrix.m[0][3];

			result.m[1][0] = m[1][0] + _matrix.m[1][0];
			result.m[1][1] = m[1][1] + _matrix.m[1][1];
			result.m[1][2] = m[1][2] + _matrix.m[1][2];
			result.m[1][3] = m[1][3] + _matrix.m[1][3];

			result.m[2][0] = m[2][0] + _matrix.m[2][0];
			result.m[2][1] = m[2][1] + _matrix.m[2][1];
			result.m[2][2] = m[2][2] + _matrix.m[2][2];
			result.m[2][3] = m[2][3] + _matrix.m[2][3];

			result.m[3][0] = m[3][0] + _matrix.m[3][0];
			result.m[3][1] = m[3][1] + _matrix.m[3][1];
			result.m[3][2] = m[3][2] + _matrix.m[3][2];
			result.m[3][3] = m[3][3] + _matrix.m[3][3];

			return result;
		}

		inline MATRIX MatrixMultiply(const MATRIX& _matrix) const
		{
			MATRIX result;
			float x = m[0][0];
			float y = m[0][1];
			float z = m[0][2];
			float w = m[0][3];
			result.m[0][0] = (_matrix.m[0][0] * x) + (_matrix.m[1][0] * y) + (_matrix.m[2][0] * z) + (_matrix.m[3][0] * w);
			result.m[0][1] = (_matrix.m[0][1] * x) + (_matrix.m[1][1] * y) + (_matrix.m[2][1] * z) + (_matrix.m[3][1] * w);
			result.m[0][2] = (_matrix.m[0][2] * x) + (_matrix.m[1][2] * y) + (_matrix.m[2][2] * z) + (_matrix.m[3][2] * w);
			result.m[0][3] = (_matrix.m[0][3] * x) + (_matrix.m[1][3] * y) + (_matrix.m[2][3] * z) + (_matrix.m[3][3] * w);
			x = m[1][0];
			y = m[1][1];
			z = m[1][2];
			w = m[1][3];
			result.m[1][0] = (_matrix.m[0][0] * x) + (_matrix.m[1][0] * y) + (_matrix.m[2][0] * z) + (_matrix.m[3][0] * w);
			result.m[1][1] = (_matrix.m[0][1] * x) + (_matrix.m[1][1] * y) + (_matrix.m[2][1] * z) + (_matrix.m[3][1] * w);
			result.m[1][2] = (_matrix.m[0][2] * x) + (_matrix.m[1][2] * y) + (_matrix.m[2][2] * z) + (_matrix.m[3][2] * w);
			result.m[1][3] = (_matrix.m[0][3] * x) + (_matrix.m[1][3] * y) + (_matrix.m[2][3] * z) + (_matrix.m[3][3] * w);
			x = m[2][0];
			y = m[2][1];
			z = m[2][2];
			w = m[2][3];
			result.m[2][0] = (_matrix.m[0][0] * x) + (_matrix.m[1][0] * y) + (_matrix.m[2][0] * z) + (_matrix.m[3][0] * w);
			result.m[2][1] = (_matrix.m[0][1] * x) + (_matrix.m[1][1] * y) + (_matrix.m[2][1] * z) + (_matrix.m[3][1] * w);
			result.m[2][2] = (_matrix.m[0][2] * x) + (_matrix.m[1][2] * y) + (_matrix.m[2][2] * z) + (_matrix.m[3][2] * w);
			result.m[2][3] = (_matrix.m[0][3] * x) + (_matrix.m[1][3] * y) + (_matrix.m[2][3] * z) + (_matrix.m[3][3] * w);
			x = m[3][0];
			y = m[3][1];
			z = m[3][2];
			w = m[3][3];
			result.m[3][0] = (_matrix.m[0][0] * x) + (_matrix.m[1][0] * y) + (_matrix.m[2][0] * z) + (_matrix.m[3][0] * w);
			result.m[3][1] = (_matrix.m[0][1] * x) + (_matrix.m[1][1] * y) + (_matrix.m[2][1] * z) + (_matrix.m[3][1] * w);
			result.m[3][2] = (_matrix.m[0][2] * x) + (_matrix.m[1][2] * y) + (_matrix.m[2][2] * z) + (_matrix.m[3][2] * w);
			result.m[3][3] = (_matrix.m[0][3] * x) + (_matrix.m[1][3] * y) + (_matrix.m[2][3] * z) + (_matrix.m[3][3] * w);
			return result;
		}

		inline MATRIX  operator* (const MATRIX& _matrix) const
		{
			return MatrixMultiply(_matrix);
		}

		inline MATRIX& operator= (const MATRIX& _matrix)
		{
			m11 = _matrix.m11; m12 = _matrix.m12; m13 = _matrix.m13; m14 = _matrix.m14;
			m21 = _matrix.m21; m22 = _matrix.m22; m23 = _matrix.m23; m24 = _matrix.m24;
			m31 = _matrix.m31; m32 = _matrix.m32; m33 = _matrix.m33; m34 = _matrix.m34;
			m41 = _matrix.m41; m42 = _matrix.m42; m43 = _matrix.m43; m44 = _matrix.m44;
			return *this;
		}

		inline MATRIX transpose(void) const {
			return MATRIX(
				m[0][0], m[1][0], m[2][0], m[3][0],
				m[0][1], m[1][1], m[2][1], m[3][1],
				m[0][2], m[1][2], m[2][2], m[3][2],
				m[0][3], m[1][3], m[2][3], m[3][3]);
		}

		static const MATRIX ZERO;
		static const MATRIX IDENTITY;
	};

	//----------------------------------------------------------------------------------------------------
	struct Point
	{
		int x, y;

		inline Point() : x(0), y(0) {}
		inline Point(int _x, int _y) : x(_x), y(_y) {}
		inline ~Point() {}

		inline Point& operator = (const Point& _point)
		{
			x = _point.x;
			y = _point.y;
			return *this;
		}
	};

	//----------------------------------------------------------------------------------------------------
	struct Rect_Int
	{
		int x, y, z, w;

	public:
		inline Rect_Int() : x(0), y(0), z(0), w(0) {}
		inline Rect_Int(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {}
		inline ~Rect_Int() {}

		inline Rect_Int& operator = (const Rect_Int& _rect)
		{
			x = _rect.x;
			y = _rect.y;
			z = _rect.z;
			w = _rect.w;
			return *this;
		}
	};

	//----------------------------------------------------------------------------------------------------
	struct Rect_Float
	{
		float x, y, z, w;

	public:
		inline Rect_Float() : x(0), y(0), z(0), w(0) {}
		inline Rect_Float(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
		inline ~Rect_Float() {}

		inline Rect_Float& operator = (const Rect_Float& _rect)
		{
			x = _rect.x;
			y = _rect.y;
			z = _rect.z;
			w = _rect.w;
			return *this;
		}
	};

	//----------------------------------------------------------------------------------------------------
	MATRIX MatrixRotationAxis(const FLOAT3& axis, const FLOAT radians);
	MATRIX MatrixRotationAxis(const FLOAT4& axis, const FLOAT radians);
	MATRIX MatrixRotationAxisN(const FLOAT3& axisN, const FLOAT radians);
	MATRIX MatrixRotationAxisN(const FLOAT4& axisN, const FLOAT radians);
	MATRIX MatrixPerspectiveFovLH(const FLOAT fovY, const FLOAT aspectRatio, const FLOAT nearZ, const FLOAT farZ);
	MATRIX MatrixTranslation(const FLOAT x, const FLOAT y, const FLOAT z);
	FLOAT MatrixMinorDeterminant(const MATRIX& _matrix,
		const UINT _r0, const UINT _r1, const UINT _r2,
		const UINT _c0, const UINT _c1, const UINT _c2);
	FLOAT MatrixDeterminant(const MATRIX& _matrix);
	MATRIX MatrixInverse(const MATRIX& _matrix);

	//----------------------------------------------------------------------------------------------------
	FLOAT3 operator* (const FLOAT3, const MATRIX);
	FLOAT4 operator* (const FLOAT4, const MATRIX);
	FLOAT3& operator*= (FLOAT3&, const MATRIX);
	FLOAT4& operator*= (FLOAT4&, const MATRIX);
}

#endif