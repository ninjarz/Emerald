#include "EEGeometry.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEQuad
	//----------------------------------------------------------------------------------------------------
	bool EEQuad::s_isQuadInitialized = false;
	ID3D11InputLayout *EEQuad::s_quadIL = NULL;
	ID3D11VertexShader *EEQuad::s_quadVS = NULL;
	ID3D11PixelShader  *EEQuad::s_quadPS = NULL;
	
	//----------------------------------------------------------------------------------------------------
	EEQuad::EEQuad(const Rect_Float &_rect)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadColor(1.0f, 1.0f, 1.0f, 1.0f),
		m_quadTex()
	{
		InitializeQuadShader();

		m_pos = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad::EEQuad(const Rect_Float &_rect, const EETexture& _tex)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadColor(1.0f, 1.0f, 1.0f, 1.0f),
		m_quadTex(_tex)
	{
		InitializeQuadShader();

		m_pos = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad::EEQuad(const Rect_Float &_rect, ID3D11ShaderResourceView* _tex)
		:
		EEObject(),
		m_quadRect(_rect),
		m_quadWidth(_rect.z - _rect.x),
		m_quadHeight(_rect.w - _rect.y),
		m_quadVB(NULL),
		m_quadColor(1.0f, 1.0f, 1.0f, 1.0f),
		m_quadTex(_tex)
	{
		InitializeQuadShader();

		m_pos = FLOAT3(_rect.x, _rect.y, 0.0f);
		CreateQuadVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEQuad::EEQuad(const EEQuad& _quad)
		:
		EEObject(_quad),
		m_quadRect(_quad.m_quadRect),
		m_quadWidth(_quad.m_quadWidth),
		m_quadHeight(_quad.m_quadHeight),
		m_quadVB(_quad.m_quadVB),
		m_quadColor(_quad.m_quadColor),
		m_quadTex(_quad.m_quadTex)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEQuad::~EEQuad()
	{
		SAFE_RELEASE(m_quadVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad::InitializeQuadShader()
	{
		if (!s_isQuadInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEQuadShader.hlsl", NULL, NULL, "QuadVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
					MessageBox(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				return FALSE;
			}
			result = D3DX11CompileFromFileW(L"EEShader\\EEQuadShader.hlsl", NULL, NULL, "QuadPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
					MessageBox(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
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
	bool EEQuad::Update()
	{
		if (m_isPosDirty || m_isScaleDirty || m_isLocalZOrderDirty)
		{
			FLOAT3 currScale = (m_scale - 1.0f) * 0.5f;
			float quadPosX = ((m_quadRect.x - m_quadWidth * currScale.x) / (float)EECore::s_EECore->GetWidth()) * 2.0f - 1.0f;
			float quadPosY = 1.0f - ((m_quadRect.y - m_quadHeight * currScale.y) / (float)EECore::s_EECore->GetHeight()) * 2.0f;
			float quadPosZ = ((m_quadRect.z + m_quadWidth * currScale.x) / (float)EECore::s_EECore->GetWidth()) * 2.0f - 1.0f;
			float quadPosW = 1.0f - ((m_quadRect.w + m_quadHeight * currScale.y) / (float)EECore::s_EECore->GetHeight()) * 2.0f;

			EEQuadVertex quadVertices[4];
			quadVertices[0].pos = FLOAT3(quadPosX, quadPosY, m_localZOrder * 0.0001f);
			quadVertices[0].tex = FLOAT2(0, 0);
			quadVertices[1].pos = FLOAT3(quadPosZ, quadPosY, m_localZOrder * 0.0001f);
			quadVertices[1].tex = FLOAT2(1, 0);
			quadVertices[2].pos = FLOAT3(quadPosX, quadPosW, m_localZOrder * 0.0001f);
			quadVertices[2].tex = FLOAT2(0, 1);
			quadVertices[3].pos = FLOAT3(quadPosZ, quadPosW, m_localZOrder * 0.0001f);
			quadVertices[3].tex = FLOAT2(1, 1);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_quadVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, quadVertices, sizeof(quadVertices));
			deviceContext->Unmap(m_quadVB, 0);

			m_isPosDirty = false;
			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad::Render()
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
	void EEQuad::SetPositionX(float _posX)
	{
		m_pos.x = _posX;
		m_quadRect.x = _posX;
		m_quadRect.z = m_quadRect.x + m_quadWidth;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad::SetPositionY(float _posY)
	{
		m_pos.y = _posY;
		m_quadRect.y = _posY;
		m_quadRect.w = m_quadRect.y + m_quadHeight;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad::SetPositionXY(const FLOAT2& _pos)
	{
		m_pos.x = _pos.x;
		m_quadRect.x = _pos.x;
		m_quadRect.z = m_quadRect.x + m_quadWidth;
		m_pos.y = _pos.y;
		m_quadRect.y = _pos.y;
		m_quadRect.w = m_quadRect.y + m_quadHeight;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEQuad::SetPosition(const FLOAT3& _position)
	{
		m_pos = _position;
		m_quadRect.x = _position.x;
		m_quadRect.z = m_quadRect.x + m_quadWidth;
		m_quadRect.y = _position.y;
		m_quadRect.w = m_quadRect.y + m_quadHeight;
		m_isPosDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad::SetTexture(EETexture* _tex)
	{
		m_quadTex.SetTexture(_tex->GetTexture());

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad::SetTexture(ID3D11ShaderResourceView* _tex)
	{
		m_quadTex.SetTexture(_tex);

		return true;
	}


	//----------------------------------------------------------------------------------------------------
	EETexture* EEQuad::GetTexture()
	{
		return &m_quadTex;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEQuad::CreateQuadVertexBuffer()
	{
		SAFE_RELEASE(m_quadVB);

		FLOAT3 currScale = (m_scale - 1.0f) * 0.5f;
		float quadPosX = ((m_quadRect.x - m_quadWidth * currScale.x) / (float)EECore::s_EECore->GetWidth()) * 2.0f - 1.0f;
		float quadPosY = 1.0f - ((m_quadRect.y - m_quadHeight * currScale.y) / (float)EECore::s_EECore->GetHeight()) * 2.0f;
		float quadPosZ = ((m_quadRect.z + m_quadWidth * currScale.x) / (float)EECore::s_EECore->GetWidth()) * 2.0f - 1.0f;
		float quadPosW = 1.0f - ((m_quadRect.w + m_quadHeight * currScale.y) / (float)EECore::s_EECore->GetHeight()) * 2.0f;

		EEQuadVertex quadVertices[4];
		quadVertices[0].pos = FLOAT3(quadPosX, quadPosY, m_localZOrder * 0.0001f);
		quadVertices[0].tex = FLOAT2(0, 0);
		quadVertices[1].pos = FLOAT3(quadPosZ, quadPosY, m_localZOrder * 0.0001f);
		quadVertices[1].tex = FLOAT2(1, 0);
		quadVertices[2].pos = FLOAT3(quadPosX, quadPosW, m_localZOrder * 0.0001f);
		quadVertices[2].tex = FLOAT2(0, 1);
		quadVertices[3].pos = FLOAT3(quadPosZ, quadPosW, m_localZOrder * 0.0001f);
		quadVertices[3].tex = FLOAT2(1, 1);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEQuadVertex)* 4;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = quadVertices;
		vbData.SysMemPitch = 0;
		vbData.SysMemSlicePitch = 0;

		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, &vbData, &m_quadVB)))
		{
			MessageBox(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}