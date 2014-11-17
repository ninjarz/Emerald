#pragma once
#ifndef _EE_OBJECT_H_
#define _EE_OBJECT_H_

#include <d3d11.h>
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEObjectBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEObjectBufferDesc
	{
		MATRIX rotation;
		float alpha;
		float tmp1;
		float tmp2;
		float tmp3;
	};

	//EEObject
	//----------------------------------------------------------------------------------------------------
	class EEObject
	{
	public:
		static bool InitializeObjectBuffer();

	public:
		EEObject();
		EEObject(const EEObject& _object);
		~EEObject();

		virtual bool Update() = NULL;
		virtual bool Render() = NULL;
		virtual bool Process();

		//parent
		virtual void SetParent(EEObject* _parent);
		//position
		virtual void SetPositionX(float _posX);
		virtual void SetPositionY(float _posY);
		virtual void SetPositionZ(float _posZ);
		virtual void SetPositionXY(const FLOAT2& _pos);
		virtual void SetPosition(const FLOAT3& _pos);
		//scale
		virtual void SetScaleX(float _scaleX);
		virtual void SetScaleY(float _scaleY);
		virtual void SetScaleZ(float _scaleZ);
		virtual void SetScale(float _scale);
		virtual void SetScale(const FLOAT3& _scale);
		//alpha
		virtual void SetAlpha(float _alpha);
		//rotation
		virtual void SetRotation(const MATRIX& _rotation);
		//localZOrder
		virtual void SetLocalZOrder(float _localZOrder);

		//parent
		virtual EEObject* GetParent();
		//position
		virtual float GetPositionX() const;
		virtual float GetPositionY() const;
		virtual float GetPositionZ() const;
		virtual const FLOAT2& GetPositionXY() const;
		virtual const FLOAT3& GetPosition() const;
		virtual bool IsPosDirty();
		virtual FLOAT3 GetCenter() = NULL;
		//scale
		virtual float GetScaleX() const;
		virtual float GetScaleY() const;
		virtual float GetScaleZ() const;
		virtual const FLOAT3& GetScale() const;
		virtual bool IsScaleDirty();
		//alpha
		virtual float GetAlpha();
		virtual bool IsAlphaDirty();
		//rotation
		virtual const MATRIX& GetRotation();
		virtual bool IsRotaionDirty();
		//localZOrder
		virtual float GetLocalZOrder();
		virtual bool IsLocalZOrderDirty();

		//position
		virtual FLOAT3 GetFinalPosition() const;
		virtual FLOAT3 GetFinalCenter() = NULL;
		//scale
		virtual FLOAT3 GetFinalScale();
		//alpha
		virtual FLOAT GetFinalAlpha();
		//rotation
		virtual MATRIX GetFinalRotation();
		//localZOrder
		virtual float GetFinalLocalZOrder();

	protected:
		virtual bool MapObjectBuffer();
		virtual bool MapObjectBuffer(float _alpha);

	protected:
		static bool s_isObjectInitialized;
		static ID3D11Buffer *s_objectBuffer;

		EEObject *m_parent;
		FLOAT3 m_position;
		bool m_isPositionDirty;
		FLOAT3 m_scale;
		bool m_isScaleDirty;
		float m_alpha;
		bool m_isAlphaDirty;
		MATRIX m_rotation;
		bool m_isRotationDirty;
		float m_localZOrder;
		bool m_isLocalZOrderDirty;
	};

	//EEObject_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEProcess(EEObject* _object);

}

#endif