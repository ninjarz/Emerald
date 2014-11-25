#include "EEGeometry.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEQuad2D
	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::s_isQuadInitialized = false;
	ID3D11InputLayout *EEQuad2D::s_quadIL = NULL;
	ID3D11VertexShader *EEQuad2D::s_quadVS = NULL;
	ID3D11PixelShader  *EEQuad2D::s_quadPS = NULL;
	
	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::InitializeQuadShader()
	{
		if (!s_isQuadInitialized)
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

			s_isQuadInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position)
		:
		EEObject(),
		m_quadRect(_position.x, _position.y, _position.x, _position.y),
		m_quadWidth(0.0f),
		m_quadHeight(0.0f),
		m_quadVB(NULL),
		m_quadTex()
	{
		InitializeQuadShader();

		m_position = _position;
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height)
		:
		EEObject(),
		m_quadRect(_position.x, _position.y, _position.x + _width, _position.y + _height),
		m_quadWidth(_width),
		m_quadHeight(_height),
		m_quadVB(NULL),
		m_quadTex()
	{
		InitializeQuadShader();

		m_position = _position;
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadTex()
	{
		InitializeQuadShader();

		m_position = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const FLOAT3& _position, FLOAT _width, FLOAT _height, const EETexture& _tex)
		:
		EEObject(),
		m_quadRect(_position.x, _position.y, _position.x + _width, _position.y + _height),
		m_quadWidth(_width),
		m_quadHeight(_height),
		m_quadVB(NULL),
		m_quadTex(_tex)
	{
		InitializeQuadShader();

		m_position = _position;
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect, const EETexture& _tex)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadTex(_tex)
	{
		InitializeQuadShader();

		m_position = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const Rect_Float &_rect, ID3D11ShaderResourceView* _tex)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadTex(_tex)
	{
		InitializeQuadShader();

		m_position = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::EEQuad2D(const EEQuad2D& _quad)
		:
		EEObject(_quad),
		m_quadRect(_quad.m_quadRect),
		m_quadWidth(_quad.m_quadWidth),
		m_quadHeight(_quad.m_quadHeight),
		m_quadVB(_quad.m_quadVB),
		m_quadTex(_quad.m_quadTex)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEQuad2D::~EEQuad2D()
	{
		SAFE_RELEASE(m_quadVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::Update()
	{
		if (m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty)
		{
			Rect_Float finalRect = GetFinalRect();

			EEQuadVertex quadVertices[4];
			quadVertices[0].pos = FLOAT3(finalRect.x, finalRect.y, m_localZOrder * 0.0001f);
			quadVertices[0].tex = FLOAT2(0, 0);
			quadVertices[1].pos = FLOAT3(finalRect.z, finalRect.y, m_localZOrder * 0.0001f);
			quadVertices[1].tex = FLOAT2(1, 0);
			quadVertices[2].pos = FLOAT3(finalRect.x, finalRect.w, m_localZOrder * 0.0001f);
			quadVertices[2].tex = FLOAT2(0, 1);
			quadVertices[3].pos = FLOAT3(finalRect.z, finalRect.w, m_localZOrder * 0.0001f);
			quadVertices[3].tex = FLOAT2(1, 1);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, quadVertices, sizeof(quadVertices));
			deviceContext->Unmap(m_quadVB, 0);

			m_isPositionDirty = false;
			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::Render()
	{
		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceConstext->IASetInputLayout(s_quadIL);
		UINT stride = sizeof(EEQuadVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_quadVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_quadVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_quadTex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_quadPS, NULL, 0);
		deviceConstext->Draw(4, 0);

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
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetWidth(float _width)
	{
		m_quadWidth = _width;
		m_quadRect.z = m_quadRect.x + _width;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad2D::SetHeight(float _height)
	{
		m_quadHeight = _height;
		m_quadRect.w = m_quadRect.y + _height;
		m_isPositionDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::SetTexture(const EETexture& _tex)
	{
		m_quadTex = _tex;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad2D::SetTexture(ID3D11ShaderResourceView* _tex)
	{
		m_quadTex.SetTexture(_tex);

		return true;
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
	EETexture* EEQuad2D::GetTexture()
	{
		return &m_quadTex;
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
	bool EEQuad2D::CreateQuadVertexBuffer(int _verticesNum)
	{
		SAFE_RELEASE(m_quadVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEQuadVertex) * _verticesNum;
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