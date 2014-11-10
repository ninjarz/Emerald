#include "EEAction.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAction_APIs
	//----------------------------------------------------------------------------------------------------
	void EEMoveBy(EEObject* _object, float _time, const FLOAT2& _dir)
	{
		boost::thread thrd(boost::bind(&EEMoveByProcess, _object, _time, _dir, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EEMoveByProcess(EEObject* _object, float _time, const FLOAT2& _dir, float _startTime)
	{
		float remainTime = _time;
		float progress = _startTime;
		FLOAT2 speed = _dir / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime)
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
	void EEScale(EEObject* _object, float _time, float _degree)
	{
		boost::thread thrd(boost::bind(&EEScaleProcess, _object, _time, _degree, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EEScaleProcess(EEObject* _object, float _time, float _degree, float _startTime)
	{
		float remainTime = _time;
		float progress = _startTime;
		FLOAT3 speed = _object->GetScale() * (_degree - 1.0f) / _time;

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
	void EERotate(EEObject* _object, float _time, float _radians)
	{
		boost::thread thrd(boost::bind(&EERotateProcess, _object, _time, _radians, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	//need to be recasted
	void EERotateProcess(EEObject* _object, float _time, float _radians, float _startTime)
	{
		float remainTime = _time;
		float progress = _startTime;
		float speed = _radians / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime)
			{
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * remainTime);
				_object->SetPosition(_object->GetPosition() * rotation);
				remainTime = 0.f;
				return;
			}
			else
			{
				MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 0.0f, 1.0f), speed * deltaTime);
				_object->SetPosition(_object->GetPosition() * rotation);
				remainTime -= deltaTime;
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EERotateBy(EEObject* _object, float _time, float _radians, const FLOAT2& _center)
	{
		boost::thread thrd(boost::bind(&EERotateByProcess, _object, _time, _radians, _center, (float)EECore::s_EECore->GetTotalTime()));
	}

	//----------------------------------------------------------------------------------------------------
	void EERotateByProcess(EEObject* _object, float _time, float _radians, const FLOAT2& _center, float _startTime)
	{
		float remainTime = _time;
		float progress = _startTime;
		float speed = _radians / _time;

		while (1)
		{
			Sleep(25);

			float currTime = (float)EECore::s_EECore->GetTotalTime();
			float deltaTime = currTime - progress;
			progress = currTime;
			if (remainTime <= deltaTime)
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
}