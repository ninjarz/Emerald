#include "EEAction.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAction_APIs
	//----------------------------------------------------------------------------------------------------
	boost::thread* EEMoveBy(EEObject* _object, float _time, const FLOAT2& _dir, float _delay, bool _isInfinite)
	{
		return new boost::thread(boost::bind(&EEMoveByProcess, _object, _time, _dir, _delay, _isInfinite, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EEMoveByProcess(EEObject* _object, float _time, const FLOAT2& _dir, float _delay, bool _isInfinite, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		FLOAT2 speed = _dir / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime && !_isInfinite)
			{
				FLOAT2 deltaPos = speed * remainTime;
				_object->SetPositionXY(_object->GetPositionXY() + deltaPos);
				remainTime = 0.f;
				return;
			}
			else
			{
				FLOAT2 deltaPos = speed * deltaTime;
				_object->SetPositionXY(_object->GetPositionXY() + deltaPos);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	boost::thread* EEScale(EEObject* _object, float _time, float _degree, float _delay)
	{
		return new boost::thread(boost::bind(&EEScaleProcess, _object, _time, _degree, _delay, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EEScaleProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		FLOAT3 speed = _object->GetScale() * (_degree - 1.0f) / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime)
			{
				FLOAT3 deltaScale = speed * remainTime;
				_object->SetScale(_object->GetScale() + deltaScale);
				remainTime = 0.f;
				return;
			}
			else
			{
				FLOAT3 deltaScale = speed * deltaTime;
				_object->SetScale(_object->GetScale() + deltaScale);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	boost::thread* EERotateYX(EEObject* _object, float _time, float _radiansYX, float _delay, bool _isInfinite)
	{
		return new boost::thread(boost::bind(&EERotateYXProcess, _object, _time, _radiansYX, _delay, _isInfinite, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EERotateYXProcess(EEObject* _object, float _time, float _radiansYX, float _delay, bool _isInfinite, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		float speed = _radiansYX / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime && !_isInfinite)
			{
				FLOAT3 finalCenter = _object->GetFinalCenter();
				MATRIX centerToOrigin = MatrixTranslation(-finalCenter.x, -finalCenter.y, 0.0f);
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * remainTime);
				MATRIX originToCenter = MatrixTranslation(finalCenter.x, finalCenter.y, 0.0f);
				_object->SetRotation(_object->GetRotation() * centerToOrigin * rotation * originToCenter);
				remainTime = 0.f;
				return;
			}
			else
			{
				FLOAT3 finalCenter = _object->GetFinalCenter();
				MATRIX centerToOrigin = MatrixTranslation(-finalCenter.x, -finalCenter.y, 0.0f);
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * deltaTime);
				MATRIX originToCenter = MatrixTranslation(finalCenter.x, finalCenter.y, 0.0f);
				_object->SetRotation(_object->GetRotation() * centerToOrigin * rotation * originToCenter);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	boost::thread* EERotate(EEObject* _object, float _time, float _radians, const FLOAT3& _center, float _delay, bool _isInfinite)
	{
		return new boost::thread(boost::bind(&EERotateProcess, _object, _time, _radians, _center, _delay, _isInfinite, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	//need to be recasted
	void EERotateProcess(EEObject* _object, float _time, float _radians, const FLOAT3& _center, float _delay, bool _isInfinite, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		float speed = _radians / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime && !_isInfinite)
			{
				MATRIX centerToOrigin = MatrixTranslation(-_center.x, -_center.y, -_center.z);
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * remainTime);
				MATRIX originToCenter = MatrixTranslation(_center.x, _center.y, _center.z);
				_object->SetRotation(_object->GetRotation() * centerToOrigin * rotation * originToCenter);
				remainTime = 0.f;
				return;
			}
			else
			{
				MATRIX centerToOrigin = MatrixTranslation(-_center.x, -_center.y, -_center.z);
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * deltaTime);
				MATRIX originToCenter = MatrixTranslation(_center.x, _center.y, _center.z);
				_object->SetRotation(_object->GetRotation() * centerToOrigin * rotation * originToCenter);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	boost::thread* EERotateBy(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay, bool _isInfinite)
	{
		return new boost::thread(boost::bind(&EERotateByProcess, _object, _time, _radians, _center, _delay, _isInfinite, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EERotateByProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _delay, bool _isInfinite, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		float speed = _radians / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime && !_isInfinite)
			{
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * remainTime);
				FLOAT3 position = _object->GetPosition() - _center;
				position *= rotation;
				_object->SetPosition(position + _center);
				remainTime = 0.f;
				return;
			}
			else
			{
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * deltaTime);
				FLOAT3 position = _object->GetPosition() - _center;
				position *= rotation;
				_object->SetPosition(position + _center);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	boost::thread* EEFade(EEObject* _object, float _time, float _degree, float _delay)
	{
		return new boost::thread(boost::bind(&EEFadeProcess, _object, _time, _degree, _delay, (float)EECore::s_EECore->GetTotalTime()));
	}

	void EEFadeProcess(EEObject* _object, float _time, float _degree, float _delay, float _startTime)
	{
		float remainTime = _delay + _time;
		float progress = _startTime;
		float speed = (_degree - _object->GetAlpha()) / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime + _time)
			{
				remainTime = _time;
				break;
			}
			else
			{
				remainTime -= deltaTime;
			}
		}

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime)
			{
				float deltaScale = speed * remainTime;
				_object->SetAlpha(_object->GetAlpha() + deltaScale);
				remainTime = 0.f;
				return;
			}
			else
			{
				float deltaScale = speed * deltaTime;
				_object->SetAlpha(_object->GetAlpha() + deltaScale);
				remainTime -= deltaTime;
			}
		}
	}
}