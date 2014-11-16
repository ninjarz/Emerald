#include "EEObject.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEObject
	//----------------------------------------------------------------------------------------------------
	bool EEObject::s_isObjectInitialized = false;
	ID3D11Buffer *EEObject::s_objectBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEObject::InitializeObjectBuffer()
	{
		if (!s_isObjectInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EEObjectBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_objectBuffer);
			if (FAILED(result))
				return false;
			deviceContext->VSSetConstantBuffers(0, 1, &s_objectBuffer);
			deviceContext->PSSetConstantBuffers(0, 1, &s_objectBuffer);

			s_isObjectInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEObject::EEObject()
		:
		m_parent(NULL),
		m_pos(0.0f),
		m_isPosDirty(false),
		m_scale(1.0f),
		m_isScaleDirty(false),
		m_alpha(1.0f),
		m_isAlphaDirty(false),
		m_rotation(MATRIX::IDENTITY),
		m_isRotationDirty(false),
		m_localZOrder(0.0f),
		m_isLocalZOrderDirty(false)
	{
		InitializeObjectBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEObject::EEObject(const EEObject& _object)
		:
		m_parent(_object.m_parent),
		m_pos(_object.m_pos),
		m_isPosDirty(_object.m_isPosDirty),
		m_scale(_object.m_scale),
		m_isScaleDirty(_object.m_isScaleDirty),
		m_alpha(_object.m_alpha),
		m_isAlphaDirty(_object.m_isAlphaDirty),
		m_localZOrder(_object.m_localZOrder),
		m_isLocalZOrderDirty(_object.m_isLocalZOrderDirty)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEObject::~EEObject()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::Process()
	{
		Update();
		Render();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetParent(EEObject* _parent)
	{
		m_parent = _parent;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionX(float _posX)
	{
		//if they are unequal
		m_pos.x = _posX;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionY(float _posY)
	{
		m_pos.y = _posY;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionZ(float _posZ)
	{
		m_pos.z = _posZ;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionXY(const FLOAT2& _pos)
	{
		m_pos.x = _pos.x;
		m_pos.y = _pos.y;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPosition(const FLOAT3& _pos)
	{
		m_pos = _pos;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleX(float _scaleX)
	{
		m_scale.x = _scaleX;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleY(float _scaleY)
	{
		m_scale.y = _scaleY;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleZ(float _scaleZ)
	{
		m_scale.z = _scaleZ;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScale(float _scale)
	{
		m_scale = _scale;
		m_isScaleDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScale(const FLOAT3& _scale)
	{
		m_scale = _scale;
		m_isScaleDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetAlpha(float _alpha)
	{
		m_alpha = _alpha;
		m_isAlphaDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetRotation(const MATRIX& _rotation)
	{
		m_rotation = _rotation;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetLocalZOrder(float _localZOrder)
	{
		m_localZOrder = _localZOrder;
		m_isLocalZOrderDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	EEObject* EEObject::GetParent()
	{
		return m_parent;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionX() const
	{
		return m_pos.x;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionY() const
	{
		return m_pos.y;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionZ() const
	{
		return m_pos.z;
	}
	
	//----------------------------------------------------------------------------------------------------
	const FLOAT2& EEObject::GetPositionXY() const
	{
		return *(FLOAT2*)(&m_pos);
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EEObject::GetPosition() const
	{
		return m_pos;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsPosDirty()
	{
		return m_isPosDirty;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetScaleX() const
	{
		return m_scale.x;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetScaleY() const
	{
		return m_scale.y;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetScaleZ() const
	{
		return m_scale.z;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EEObject::GetScale() const
	{
		return m_scale;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsScaleDirty()
	{
		return m_isScaleDirty;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetAlpha()
	{
		return m_alpha;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsAlphaDirty()
	{
		return m_isAlphaDirty;
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EEObject::GetRotation()
	{
		return m_rotation;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsRotaionDirty()
	{
		return m_isRotationDirty;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetLocalZOrder()
	{
		return m_localZOrder;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsLocalZOrderDirty()
	{
		return m_isLocalZOrderDirty;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::MapObjectBuffer()
	{
		HRESULT result;
		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		EEObjectBufferDesc *objectBufferDesc;
		result = deviceContext->Map(s_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;
		//I don't know whether it is feasible
		objectBufferDesc = (EEObjectBufferDesc*)mappedResource.pData;
		objectBufferDesc->rotation = m_parent ? m_parent->GetRotation() * m_rotation : m_rotation;
		objectBufferDesc->alpha = m_parent ? m_parent->GetAlpha() * m_alpha : m_alpha;
		deviceContext->Unmap(s_objectBuffer, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::MapObjectBuffer(float _alpha)
	{
		HRESULT result;
		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		EEObjectBufferDesc *objectBufferDesc;
		result = deviceContext->Map(s_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;
		//I don't know whether it is feasible
		objectBufferDesc = (EEObjectBufferDesc*)mappedResource.pData;
		objectBufferDesc->rotation = m_parent ? m_parent->GetRotation() * m_rotation : m_rotation;
		objectBufferDesc->alpha = m_parent ? m_parent->GetAlpha() * _alpha : _alpha;
		deviceContext->Unmap(s_objectBuffer, 0);

		return true;
	}

	//EEObject_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEProcess(EEObject* _object)
	{
		_object->Process();

		return true;
	}
}