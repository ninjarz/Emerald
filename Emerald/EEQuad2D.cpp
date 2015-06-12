#include "EEQuad2D.h"
#include "EECore.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEQuad2D
	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::s_isQuad2DInitialized = false;
	ID3D11InputLayout *EEQuad2D::s_quadIL = NULL;
	ID3D11VertexShader *EEQuad2D::s_quadVS = NULL;
	ID3D11PixelShader  *EEQuad2D::s_quadPS = NULL;
	ID3D11Buffer *EEQuad2D::s_quad2DBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::InitializeQuad2D()
	{
		if (!s_isQuad2DInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEQuad2DShader.hlsl", NULL, NULL, "QuadVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "ShaderCompileError", MB_YESNO);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				else
				{
					MessageBoxW(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				return FALSE;
			}
			result = D3DX11CompileFromFileW(L"EEShader\\EEQuad2DShader.hlsl", NULL, NULL, "QuadPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "ShaderCompileError", MB_YESNO);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				else
				{
					MessageBoxW(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				return FALSE;
			}
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_quadVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_quadPS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC quadInputDesc[2] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(quadInputDesc, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_quadIL);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}

			SAFE_RELEASE(errorMessage);
			SAFE_RELEASE(vertexShaderBuffer);
			SAFE_RELEASE(pixelShaderBuffer);

			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			s_isQuad2DInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D()
		:
		EEObject2D(),
		m_quadRect(),
		m_quadWidth(0.0f),
		m_quadHeight(0.0f),
		m_quadVB(NULL)
	{
		InitializeQuad2D();

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position)
		:
		EEObject2D(_position),
		m_quadRect(_position.x, _position.y, _position.x, _position.y),
		m_quadWidth(0.0f),
		m_quadHeight(0.0f),
		m_quadVB(NULL)
	{
		InitializeQuad2D();

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height)
		:
		EEObject2D(_position),
		m_quadRect(_position.x, _position.y, _position.x + _width, _position.y + _height),
		m_quadWidth(_width),
		m_quadHeight(_height),
		m_quadVB(NULL)
	{
		InitializeQuad2D();

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height, const EETexture& _tex)
		:
		EEObject2D(_position),
		m_quadRect(_position.x, _position.y, _position.x + _width, _position.y + _height),
		m_quadWidth(_width),
		m_quadHeight(_height),
		m_quadVB(NULL)
	{
		InitializeQuad2D();
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect)
		:
		EEObject2D(FLOAT3(_rect.x, _rect.y, 0.0f)),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL)
	{
		InitializeQuad2D();

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float& _rect, const EEColor& _color)
		:
		EEObject2D(FLOAT3(_rect.x, _rect.y, 0.0f)),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL)
	{
		InitializeQuad2D();

		SetColor(_color);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect, const EETexture& _tex)
		:
		EEObject2D(FLOAT3(_rect.x, _rect.y, 0.0f)),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL)
	{
		InitializeQuad2D();
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect, ID3D11ShaderResourceView* _tex)
		:
		EEObject2D(FLOAT3(_rect.x, _rect.y, 0.0f)),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL)
	{
		InitializeQuad2D();
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const EEQuad2D& _quad)
		:
		EEObject2D(_quad),
		m_quadRect(_quad.m_quadRect),
		m_quadWidth(_quad.m_quadWidth),
		m_quadHeight(_quad.m_quadHeight),
		m_quadVB(nullptr)
	{
		D3D11_BUFFER_DESC bufferDesc;
		_quad.m_quadVB->GetDesc(&bufferDesc);
		if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateBuffer(&bufferDesc, NULL, &m_quadVB)))
		{
			EECore::s_EECore->GetDeviceContext()->CopyResource(m_quadVB, _quad.m_quadVB);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::~EEQuad2D()
	{
		SAFE_RELEASE(m_quadVB);
		if (s_focusedObject == this)
			s_focusedObject = nullptr;
		if (s_triggeredObject == this)
			s_triggeredObject = nullptr;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty)
		{
			FLOAT3 scale = (m_parent ? m_parent->GetFinalScale() * m_scale - 1.0f : m_scale - 1.0f) * 0.5f;

			Rect_Float rect(
				-m_quadWidth * scale.x,
				-m_quadHeight * scale.y,
				m_quadWidth + m_quadWidth * scale.x,
				m_quadHeight + m_quadHeight * scale.y
				);

			EEQuad2DVertex quadVertices[4];
			quadVertices[0].pos = FLOAT3(rect.x, rect.y, m_localZOrder * 0.0001f);
			quadVertices[0].tex = FLOAT2(m_texRect.x, m_texRect.y);
			quadVertices[1].pos = FLOAT3(rect.z, rect.y, m_localZOrder * 0.0001f);
			quadVertices[1].tex = FLOAT2(m_texRect.z, m_texRect.y);
			quadVertices[2].pos = FLOAT3(rect.x, rect.w, m_localZOrder * 0.0001f);
			quadVertices[2].tex = FLOAT2(m_texRect.x, m_texRect.w);
			quadVertices[3].pos = FLOAT3(rect.z, rect.w, m_localZOrder * 0.0001f);
			quadVertices[3].tex = FLOAT2(m_texRect.z, m_texRect.w);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, quadVertices, sizeof(quadVertices));
			deviceContext->Unmap(m_quadVB, 0);

			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceContext->IASetInputLayout(s_quadIL);
		UINT stride = sizeof(EEQuad2DVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetShader(s_quadPS, NULL, 0);
		deviceContext->Draw(4, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetPositionX(float _posX)
	{
		m_position.x = _posX;
		m_quadRect.x = _posX;
		m_quadRect.z = _posX + m_quadWidth;

		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetPositionY(float _posY)
	{
		m_position.y = _posY;
		m_quadRect.y = _posY;
		m_quadRect.w = _posY + m_quadHeight;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetPositionXY(const FLOAT2& _position)
	{
		m_position.x = _position.x;
		m_quadRect.x = _position.x;
		m_quadRect.z = _position.x + m_quadWidth;
		m_position.y = _position.y;
		m_quadRect.y = _position.y;
		m_quadRect.w = _position.y + m_quadHeight;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetPosition(const FLOAT3& _position)
	{
		m_position = _position;
		m_quadRect.x = _position.x;
		m_quadRect.z = _position.x + m_quadWidth;
		m_quadRect.y = _position.y;
		m_quadRect.w = _position.y + m_quadHeight;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetRect(const Rect_Float& _rect)
	{
		m_position = FLOAT3(_rect.x, _rect.y, 0.0f);
		m_quadRect = _rect;
		m_quadWidth = _rect.z - _rect.x;
		m_quadHeight = _rect.w - _rect.y;
		m_isPositionDirty = true;
		m_isScaleDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetWidth(float _width)
	{
		m_quadWidth = _width;
		m_quadRect.z = m_quadRect.x + _width;
		m_isPositionDirty = true;
		m_isScaleDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetHeight(float _height)
	{
		m_quadHeight = _height;
		m_quadRect.w = m_quadRect.y + _height;
		m_isPositionDirty = true;
		m_isScaleDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	const Rect_Float& EEQuad2D::GetRect() const
	{
		return m_quadRect;
	}

	//----------------------------------------------------------------------------------------------------
	float EEQuad2D::GetWidht() const
	{
		return m_quadWidth;
	}

	//----------------------------------------------------------------------------------------------------
	float EEQuad2D::GetHeight() const
	{
		return m_quadHeight;
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEQuad2D::GetCenter() const
	{
		return FLOAT3(m_position.x + m_quadWidth / 2, m_position.y + m_quadHeight / 2, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEQuad2D::GetRowCenter() const
	{
		return FLOAT3(m_quadWidth / 2, m_quadHeight / 2, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	//this function is different with other final functions. it is embellished with scale
	Rect_Float EEQuad2D::GetFinalRect() const
	{
		if (m_parent)
		{
			FLOAT3 scale = (GetFinalScale() - 1.0f) * 0.5f;
			FLOAT3 deltaPos = m_parent->GetFinalPosition();
			return Rect_Float(
				m_quadRect.x - m_quadWidth * scale.x + deltaPos.x,
				m_quadRect.y - m_quadHeight * scale.y + deltaPos.y,
				m_quadRect.z + m_quadWidth * scale.x + deltaPos.x,
				m_quadRect.w + m_quadHeight * scale.y + deltaPos.y
				);
		}
		else
		{
			FLOAT3 scale = (m_scale - 1.0f) * 0.5f;
			return Rect_Float(
				m_quadRect.x - m_quadWidth * scale.x,
				m_quadRect.y - m_quadHeight * scale.y,
				m_quadRect.z + m_quadWidth * scale.x,
				m_quadRect.w + m_quadHeight * scale.y
				);
		}
	}

	//----------------------------------------------------------------------------------------------------
	FLOAT3 EEQuad2D::GetFinalCenter() const
	{
		FLOAT3 finalPosition = GetFinalPosition();
		return FLOAT3(finalPosition.x + m_quadWidth / 2, finalPosition.y + m_quadHeight / 2, 0.0f);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::UpdateObjectState()
	{
		// the mouse is within the rect
		// GetFinalRect should be optimized!
		if (EECollision(GetFinalRect(), EECore::s_EECore->GetMousePosition()))
		{
			// DOWN
			if (EECore::s_EECore->IsKeyDown(VK_LBUTTON))
			{
				if (m_state == EE_OBJECT_OVER && s_focusedObject == nullptr && m_isFocusable)
					OnMouseClicked(EECore::s_EECore->GetMousePosition());
			}
			// DOWN TO UP
			else if (m_state == EE_OBJECT_DOWN && s_focusedObject == this && m_isFocusable)
			{
				OnMouseTriggered(EECore::s_EECore->GetMousePosition());
			}
			// OVER
			else
			{
				OnMouseOver(EECore::s_EECore->GetMousePosition());
			}
		}
		else
		{
			if (s_focusedObject == this && !EECore::s_EECore->IsKeyDown(VK_LBUTTON))
				s_focusedObject = nullptr;
			else if (s_focusedObject != this)
				OnMouseFree(EECore::s_EECore->GetMousePosition());
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::CreateQuadVertexBuffer(int _verticesNum)
	{
		SAFE_RELEASE(m_quadVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEQuad2DVertex)* _verticesNum;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_quadVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}