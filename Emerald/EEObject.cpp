#include "EEObject.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEObject
	//----------------------------------------------------------------------------------------------------
	bool EEObject::s_isObjectInitialized = false;
	ID3D11Buffer *EEObject::s_objectBuffer = NULL;
	EEObject *EEObject::s_focusedObject = NULL;
	EEObject *EEObject::s_triggeredObject = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEObject::InitializeObject()
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

			s_isObjectInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEObject::EEObject()
		:
		m_isAlive(true),
		m_parent(NULL),
		m_position(0.0f),
		m_isPositionDirty(true),
		m_scale(1.0f),
		m_isScaleDirty(true),
		m_alpha(1.0f),
		m_isAlphaDirty(true),
		m_rotation(MATRIX::IDENTITY),
		m_isRotationDirty(true),
		m_color(0.0f, 0.0f, 0.0f, 0.0f),
		m_isColorDirty(true),
		m_isUseColor(true),
		m_tex(),
		m_texIndex(0),
		m_texRect(0.0, 0.0, 1.0f, 1.0f),
		m_isTexRectDirty(true),
		m_isUseTex(false),
		m_localZOrder(0.0f),
		m_isLocalZOrderDirty(true),
		//state
		m_isFocusable(false),
		m_state(EE_OBJECT_FREE)
	{
		InitializeObject();
	}

	//----------------------------------------------------------------------------------------------------
	EEObject::EEObject(const FLOAT3& _position)
		:
		m_isAlive(true),
		m_parent(NULL),
		m_position(_position),
		m_isPositionDirty(true),
		m_scale(1.0f),
		m_isScaleDirty(true),
		m_alpha(1.0f),
		m_isAlphaDirty(true),
		m_rotation(MATRIX::IDENTITY),
		m_isRotationDirty(true),
		m_color(0.0f, 0.0f, 0.0f, 0.0f),
		m_isColorDirty(true),
		m_isUseColor(true),
		m_tex(),
		m_texIndex(0),
		m_texRect(0.0, 0.0, 1.0f, 1.0f),
		m_isTexRectDirty(true),
		m_isUseTex(false),
		m_localZOrder(0.0f),
		m_isLocalZOrderDirty(true),
		//state
		m_isFocusable(false),
		m_state(EE_OBJECT_FREE)
	{
		InitializeObject();
	}

	//----------------------------------------------------------------------------------------------------
	EEObject::EEObject(const EEObject& _object)
		:
		m_isAlive(_object.m_isAlive),
		m_parent(_object.m_parent),
		m_position(_object.m_position),
		m_isPositionDirty(_object.m_isPositionDirty),
		m_scale(_object.m_scale),
		m_isScaleDirty(_object.m_isScaleDirty),
		m_alpha(_object.m_alpha),
		m_isAlphaDirty(_object.m_isAlphaDirty),
		m_color(_object.m_color),
		m_isColorDirty(_object.m_isColorDirty),
		m_isUseColor(_object.m_isUseColor),
		m_tex(_object.m_tex),
		m_texIndex(_object.m_texIndex),
		m_texRect(_object.m_texRect),
		m_isTexRectDirty(_object.m_isTexRectDirty),
		m_isUseTex(_object.m_isUseTex),
		m_localZOrder(_object.m_localZOrder),
		m_isLocalZOrderDirty(_object.m_isLocalZOrderDirty),
		//state
		m_isFocusable(false),
		m_state(_object.m_state)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEObject::~EEObject()
	{
		RemoveThread();

		if (m_parent)
		{
			m_parent->m_children.erase(this);
		}

		ClearChildren();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::Update()
	{
		if (m_isAlive)
		{
			OnUpdate();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::Render()
	{
		if (m_isAlive)
		{
			OnRender();
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::Process()
	{
		bool result = false;
		ThreadLock();

		if (Update())
			result = Render();

		ThreadUnLock();
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetIsAlive(bool _isAlive)
	{
		m_isAlive = _isAlive;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetParent(EEObject* _parent)
	{
		if (m_parent)
			m_parent->m_children.erase(this);

		m_parent = _parent;
		if (m_parent)
			m_parent->m_children[this] = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::ClearChildren()
	{
		for (auto& child : m_children)
		{
			if (child.first)
			{
				child.first->SetParent(nullptr);
			}
		}
		m_children.clear();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionX(float _posX)
	{
		//if they are unequal
		m_position.x = _posX;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionY(float _posY)
	{
		m_position.y = _posY;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionZ(float _posZ)
	{
		m_position.z = _posZ;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPositionXY(const FLOAT2& _pos)
	{
		m_position.x = _pos.x;
		m_position.y = _pos.y;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetPosition(const FLOAT3& _pos)
	{
		m_position = _pos;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleX(float _scaleX)
	{
		m_scale.x = _scaleX;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleY(float _scaleY)
	{
		m_scale.y = _scaleY;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetScaleZ(float _scaleZ)
	{
		m_scale.z = _scaleZ;
		m_isPositionDirty = true;
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
	void EEObject::SetColor(const EEColor& _color)
	{
		m_color = _color;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetIsUseColor(bool _isUseColor)
	{
		m_isUseColor = _isUseColor;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTexture(const EETexture& _tex)
	{
		m_tex = _tex;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTexture(ID3D11ShaderResourceView* _tex)
	{
		m_tex.SetTexture(_tex);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTexture(EETexture* _texs, int _amount)
	{
		return SetTexture(EETextureCombine(_texs, _amount));
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTexIndex(int _index)
	{
		m_texIndex = _index;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTexRect(Rect_Float& _texRect)
	{
		m_texRect = _texRect;
		m_isTexRectDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetIsUseTex(bool _isUseTex)
	{
		m_isUseTex = _isUseTex;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetLocalZOrder(float _localZOrder)
	{
		m_localZOrder = _localZOrder;
		m_isLocalZOrderDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::SetIsFocusable(bool _isFocusable)
	{
		m_isFocusable = _isFocusable;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetUpdateFunc(std::function<void(void)> _funcPtr)
	{
		m_updateFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetFreeFunc(std::function<void(void)> _funcPtr)
	{
		m_freeFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetOverFunc(std::function<void(void)> _funcPtr)
	{
		m_overFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetClickedFunc(std::function<void(void)> _funcPtr)
	{
		m_clickedFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::SetTriggeredFunc(std::function<void(void)> _funcPtr)
	{
		m_TriggeredFunc = _funcPtr;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::AddThread(boost::thread* _thread)
	{
		m_threads.push_back(_thread);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::RemoveThread()
	{
		for (boost::thread* thread : m_threads)
		{
			thread->interrupt();
			thread->join();
			delete thread;
		}
		m_threads.clear();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsAlive()
	{
		return m_isAlive;
	}

	//----------------------------------------------------------------------------------------------------
	EEObject* EEObject::GetParent()
	{
		return m_parent;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionX() const
	{
		return m_position.x;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionY() const
	{
		return m_position.y;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetPositionZ() const
	{
		return m_position.z;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT2& EEObject::GetPositionXY() const
	{
		return *(FLOAT2*)(&m_position);
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EEObject::GetPosition() const
	{
		return m_position;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsPositionDirty() const
	{
		return m_isPositionDirty;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEObject::GetCenter() const
	{
		return m_position;
	}

	FLOAT3 EEObject::GetRowCenter() const
	{
		return FLOAT3(0.0f, 0.0f, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetOffsetWidth() const
	{
		float result = m_position.x; // self, should be rewrote
		for (auto& child : m_children) // children
		{
			if (child.first)
			{
				float offsetWidth = m_position.x + child.first->GetOffsetWidth();
				if (result < offsetWidth)
					result = offsetWidth;
			}
		}

		return result;
	}

	float EEObject::GetOffsetHeight() const
	{
		float result = m_position.y; // self, should be rewrote
		for (auto& child : m_children) // children
		{
			if (child.first)
			{
				float offsetHeight = m_position.y + child.first->GetOffsetHeight();
				if (result < offsetHeight)
					result = offsetHeight;
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject::GetWorldMatrix()
	{
		return MatrixTranslation(GetPosition());
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject::GetViewMatrix()
	{
		return EECore::s_EECore->GetViewMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject::GetProjectionMatrix()
	{
		return EECore::s_EECore->GetProjectionMatrix();
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
	bool EEObject::IsScaleDirty() const
	{
		return m_isScaleDirty;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetAlpha() const
	{
		return m_alpha;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsAlphaDirty() const
	{
		return m_isAlphaDirty;
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EEObject::GetRotation() const
	{
		return m_rotation;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsRotaionDirty() const
	{
		return m_isRotationDirty;
	}

	//----------------------------------------------------------------------------------------------------
	const EEColor& EEObject::GetColor() const
	{
		return m_color;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsColorDirty() const
	{
		return m_isColorDirty;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsUseColor() const
	{
		return m_isUseColor;
	}

	//----------------------------------------------------------------------------------------------------
	EETexture* EEObject::GetTexture()
	{
		return &m_tex;
	}

	//----------------------------------------------------------------------------------------------------
	int EEObject::GetTexIndex()
	{
		return m_texIndex;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsUseTex() const
	{
		return m_isUseTex;
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetLocalZOrder() const
	{
		return m_localZOrder;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::IsLocalZOrderDirty() const
	{
		return m_isLocalZOrderDirty;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEObject::GetFinalPosition() const
	{
		if (m_parent)
		{
			return m_position + m_parent->GetFinalPosition();
		}
		else
		{
			return m_position;
		}
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEObject::GetFinalCenter() const
	{
		if (m_parent)
		{
			return m_position + m_parent->GetFinalPosition();
		}
		else
		{
			return m_position;
		}
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject::GetFinalWorldMatrix() const
	{
		return MatrixScaling(GetFinalScale()) * MatrixTranslation(GetFinalPosition());
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEObject::GetFinalScale() const
	{
		if (m_parent)
		{
			return m_scale * m_parent->GetFinalScale();
		}
		else
		{
			return m_scale;
		}
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT EEObject::GetFinalAlpha() const
	{
		if (m_parent)
		{
			return m_alpha * m_parent->GetFinalAlpha();
		}
		else
		{
			return m_alpha;
		}
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject::GetFinalRotation() const
	{
		if (m_parent)
		{
			return m_rotation * m_parent->GetFinalRotation();
		}
		else
		{
			return m_rotation;
		}
	}

	//----------------------------------------------------------------------------------------------------
	float EEObject::GetFinalLocalZOrder() const
	{
		if (m_parent)
		{
			return m_parent->GetFinalLocalZOrder();
		}
		else
		{
			return m_localZOrder;
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnUpdate()
	{
		if (m_updateFunc)
			m_updateFunc();
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnRender()
	{
		if (m_renderFunc)
			m_renderFunc();
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnMouseFree(const Point& _pos)
	{
		m_state = EE_OBJECT_FREE;
		if (m_freeFunc)
			m_freeFunc();
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnMouseOver(const Point& _pos)
	{
		m_state = EE_OBJECT_OVER;
		if (m_overFunc)
			m_overFunc();
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnMouseClicked(const Point& _pos)
	{
		m_state = EE_OBJECT_DOWN;
		if (m_clickedFunc)
			m_clickedFunc();
		s_focusedObject = this;
		s_triggeredObject = nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::OnMouseTriggered(const Point& _pos)
	{
		m_state = EE_OBJECT_OVER;
		if (m_TriggeredFunc)
			m_TriggeredFunc();
		s_focusedObject = nullptr;
		s_triggeredObject = this;
		//m_isTriggered = true;
	}

	//----------------------------------------------------------------------------------------------------
	std::mutex& EEObject::GetThreadMutex()
	{
		return m_threadMutex;
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::ThreadLock()
	{
		m_threadMutex.lock();
	}

	//----------------------------------------------------------------------------------------------------
	void EEObject::ThreadUnLock()
	{
		m_threadMutex.unlock();
	}
	
	//----------------------------------------------------------------------------------------------------
	bool EEObject::operator< (const EEObject& _object) const
	{
		return m_localZOrder < _object.GetLocalZOrder();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::operator> (const EEObject& _object) const
	{
		return m_localZOrder > _object.GetLocalZOrder();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::MapObjectBuffer()
	{
		EECore::s_EECore->MapCameraBuffer();

		HRESULT result;
		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		EEObjectBufferDesc *objectBufferDesc;
		result = deviceContext->Map(s_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;
		objectBufferDesc = (EEObjectBufferDesc*)mappedResource.pData;
		objectBufferDesc->worldMatrix = GetFinalWorldMatrix();
		objectBufferDesc->worldViewProjMatrix = GetFinalWorldMatrix() * GetFinalRotation() * GetViewMatrix() * GetProjectionMatrix();
		objectBufferDesc->rotationMatrix = GetFinalRotation();
		objectBufferDesc->color = GetColor();
		objectBufferDesc->texIndex = GetTexIndex();
		objectBufferDesc->isUseColor = IsUseColor();
		objectBufferDesc->isUseTex = IsUseTex();
		objectBufferDesc->alpha = GetFinalAlpha();
		deviceContext->Unmap(s_objectBuffer, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &s_objectBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &s_objectBuffer);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::MapObjectBuffer(float _alpha)
	{
		EECore::s_EECore->MapCameraBuffer();

		HRESULT result;
		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		EEObjectBufferDesc *objectBufferDesc;
		result = deviceContext->Map(s_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result))
			return false;
		objectBufferDesc = (EEObjectBufferDesc*)mappedResource.pData;
		objectBufferDesc->worldMatrix = GetFinalWorldMatrix();
		objectBufferDesc->worldViewProjMatrix = GetFinalWorldMatrix() * GetFinalRotation() * GetViewMatrix() * GetProjectionMatrix();
		objectBufferDesc->rotationMatrix = GetFinalRotation();
		objectBufferDesc->color = GetColor();
		objectBufferDesc->texIndex = GetTexIndex();
		objectBufferDesc->isUseColor = IsUseColor();
		objectBufferDesc->isUseTex = IsUseTex();
		objectBufferDesc->alpha = _alpha;
		deviceContext->Unmap(s_objectBuffer, 0);

		deviceContext->VSSetConstantBuffers(0, 1, &s_objectBuffer);
		deviceContext->PSSetConstantBuffers(0, 1, &s_objectBuffer);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEObject::UpdateObjectState()
	{
		//virtual
		return true;
	}

	//EEObject_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEProcess(EEObject* _object)
	{
		return _object->Process();
	}
}