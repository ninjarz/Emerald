#pragma once
#ifndef _EE_OBJECT_H_
#define _EE_OBJECT_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include <functional>
#include <boost/thread/thread.hpp>
#include "EEMath.h"
#include "EEColor.h"
#include "EETexture.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEObjectBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEObjectBufferDesc
	{
		MATRIX worldMatrix;
		MATRIX worldViewProjMatrix;
		MATRIX rotationMatrix;
		FLOAT4 color;
		int texIndex;
		int isUseColor;
		int isUseTex;
		float alpha;
	};

	// EEObjectState
	//----------------------------------------------------------------------------------------------------
	typedef enum
	{
		EE_OBJECT_FREE = 0,
		EE_OBJECT_DOWN = 1,
		EE_OBJECT_OVER = 2,
	} EEObjectState;

	// EEObject
	//----------------------------------------------------------------------------------------------------
	class EEObject
	{
	protected:
		static bool InitializeObject();

	protected:
		static bool s_isObjectInitialized;
		static ID3D11Buffer *s_objectBuffer;
		static EEObject *s_focusedObject;
		static EEObject *s_triggeredObject;
	public:
		EEObject();
		EEObject(const FLOAT3& _position);
		EEObject(const EEObject& _object);
		virtual ~EEObject();
		virtual inline EEObject* Clone() { return new EEObject(*this); }

		virtual bool Update();
		virtual bool Render();
		virtual bool Process();

		// life
		virtual void SetIsAlive(bool _isAlive);
		// parent
		virtual void SetParent(EEObject* _parent);
		// position
		virtual void SetPositionX(float _posX);
		virtual void SetPositionY(float _posY);
		virtual void SetPositionZ(float _posZ);
		virtual void SetPositionXY(const FLOAT2& _pos);
		virtual void SetPosition(const FLOAT3& _pos);
		// scale
		virtual void SetScaleX(float _scaleX);
		virtual void SetScaleY(float _scaleY);
		virtual void SetScaleZ(float _scaleZ);
		virtual void SetScale(float _scale);
		virtual void SetScale(const FLOAT3& _scale);
		// alpha
		virtual void SetAlpha(float _alpha);
		// rotation
		virtual void SetRotation(const MATRIX& _rotation);
		// color
		virtual void SetColor(const EEColor& _color);
		virtual bool SetIsUseColor(bool _isUseColor);
		// texture
		virtual bool SetTexture(const EETexture& _tex);
		virtual bool SetTexture(ID3D11ShaderResourceView* _tex);
		virtual bool SetTexIndex(int _index);
		virtual bool SetTexRect(Rect_Float& _texRect);
		virtual bool SetIsUseTex(bool _isUseTex);
		// localZOrder
		virtual void SetLocalZOrder(float _localZOrder);
		// state
		virtual void SetIsFocusable(bool _isFocusable);
		// callback function
		virtual bool SetUpdateFunc(std::function<void(void)> _funcPtr);
		virtual bool SetFreeFunc(std::function<void(void)> _funcPtr);
		virtual bool SetOverFunc(std::function<void(void)> _funcPtr);
		virtual bool SetClickedFunc(std::function<void(void)> _funcPtr);
		virtual bool SetTriggeredFunc(std::function<void(void)> _funcPtr);
		// action
		virtual void AddThread(boost::thread* _thread);
		virtual bool RemoveThread();

		// life
		bool IsAlive();
		// parent
		virtual EEObject* GetParent();
		// position
		virtual float GetPositionX() const;
		virtual float GetPositionY() const;
		virtual float GetPositionZ() const;
		virtual const FLOAT2& GetPositionXY() const;
		virtual const FLOAT3& GetPosition() const;
		virtual bool IsPositionDirty()  const;
		virtual FLOAT3 GetCenter() const;
		virtual FLOAT3 GetRowCenter() const;
		virtual MATRIX GetWorldMatrix();
		virtual MATRIX GetViewMatrix();
		virtual MATRIX GetProjectionMatrix();
		// scale
		virtual float GetScaleX() const;
		virtual float GetScaleY() const;
		virtual float GetScaleZ() const;
		virtual const FLOAT3& GetScale() const;
		virtual bool IsScaleDirty() const;
		// alpha
		virtual float GetAlpha() const;
		virtual bool IsAlphaDirty() const;
		// rotation
		virtual const MATRIX& GetRotation() const;
		virtual bool IsRotaionDirty() const;
		// color
		virtual const EEColor& GetColor() const;
		virtual bool IsColorDirty() const;
		virtual bool IsUseColor() const;
		// texture
		virtual EETexture* GetTexture();
		virtual int GetTexIndex();
		virtual bool IsUseTex() const;
		// localZOrder
		virtual float GetLocalZOrder() const;
		virtual bool IsLocalZOrderDirty() const;

		// position
		virtual FLOAT3 GetFinalPosition() const;
		virtual FLOAT3 GetFinalCenter() const;
		virtual MATRIX GetFinalWorldMatrix() const;
		// scale
		virtual FLOAT3 GetFinalScale() const;
		// alpha
		virtual FLOAT GetFinalAlpha() const;
		// rotation
		virtual MATRIX GetFinalRotation() const;
		// localZOrder
		virtual float GetFinalLocalZOrder() const;

		// OnFunction
		virtual void OnUpdate();
		virtual void OnRender();
		virtual void OnMouseFree(const Point& _pos);
		virtual void OnMouseOver(const Point& _pos);
		virtual void OnMouseClicked(const Point& _pos);
		virtual void OnMouseTriggered(const Point& _pos);

		//operator
		bool operator< (const EEObject& _object) const;
		bool operator> (const EEObject& _object) const;

	protected:
		//cbuffer
		virtual bool MapObjectBuffer();
		virtual bool MapObjectBuffer(float _alpha);
		//state
		virtual bool UpdateObjectState();

	protected:
		// info
		bool m_isAlive;
		EEObject *m_parent;

		// translate
		FLOAT3 m_position;
		bool m_isPositionDirty;
		// 
		FLOAT3 m_scale;
		bool m_isScaleDirty;
		float m_alpha;
		bool m_isAlphaDirty;
		MATRIX m_rotation;
		bool m_isRotationDirty;
		EEColor m_color;
		bool m_isColorDirty;
		bool m_isUseColor;
		EETexture m_tex;
		int m_texIndex;
		Rect_Float m_texRect;
		bool m_isUseTex;
		float m_localZOrder;
		bool m_isLocalZOrderDirty;
		// state
		bool m_isFocusable;
		EEObjectState m_state;
		// callback function
		std::function<void(void)> m_updateFunc;
		std::function<void(void)> m_renderFunc;
		std::function<void(void)> m_freeFunc;
		std::function<void(void)> m_overFunc;
		std::function<void(void)> m_clickedFunc;
		std::function<void(void)> m_TriggeredFunc;
		// action
		std::vector<boost::thread*> m_threads;
	};

	// EEObject_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEProcess(EEObject* _object);

}

#endif