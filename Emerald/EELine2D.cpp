#include "EELine2D.h"
#include "EECore.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELine2D
	//----------------------------------------------------------------------------------------------------
	bool EELine2D::s_isLine2DInitialized = false;
	ID3D11InputLayout *EELine2D::s_lineIL = NULL;
	ID3D11VertexShader *EELine2D::s_lineVS = NULL;
	ID3D11PixelShader  *EELine2D::s_linePS = NULL;
	ID3D11Buffer *EELine2D::s_line2DBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EELine2D::InitializeLine2D()
	{
		if (!s_isLine2DInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EELine2DShader.hlsl", NULL, NULL, "LineVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
			result = D3DX11CompileFromFileW(L"EEShader\\EELine2DShader.hlsl", NULL, NULL, "LinePS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_lineVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_linePS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC inputDesc[2] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(inputDesc, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_lineIL);
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
			/*
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EELine2DBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_line2DBuffer);
			if (FAILED(result))
				return false;
				*/

			s_isLine2DInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EELine2D::EELine2D(FLOAT2& _start, FLOAT2& _end, const EEColor& _color)
		:
		EEObject2D(FLOAT3(_start, 0.0f)),
		m_pos0(_start),
		m_pos1(_end),
		m_width(10.f),
		m_lineVB(nullptr),
		m_isLineDirty(true)
	{
		InitializeLine2D();
		SetColor(_color);

		CreateLineVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EELine2D::~EELine2D()
	{
		SAFE_RELEASE(m_lineVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EELine2D::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty || m_isLineDirty)
		{
			EELine2DVertex vertices[4];
			FLOAT2 vertical = (m_pos1 - m_pos0).GetVertical();
			vertices[0].pos = FLOAT3(m_pos0 - vertical * m_width, m_localZOrder * 0.0001f);
			vertices[0].tex = FLOAT2(m_texRect.x, m_texRect.y);
			vertices[1].pos = FLOAT3(m_pos1 - vertical * m_width, m_localZOrder * 0.0001f);
			vertices[1].tex = FLOAT2(m_texRect.z, m_texRect.y);
			vertices[2].pos = FLOAT3(m_pos0 + vertical * m_width, m_localZOrder * 0.0001f);
			vertices[2].tex = FLOAT2(m_texRect.x, m_texRect.w);
			vertices[3].pos = FLOAT3(m_pos1 + vertical * m_width, m_localZOrder * 0.0001f);
			vertices[3].tex = FLOAT2(m_texRect.z, m_texRect.w);

			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_lineVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, vertices, sizeof(vertices));
			deviceContext->Unmap(m_lineVB, 0);

			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
			m_isLineDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELine2D::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceContext->IASetInputLayout(s_lineIL);
		UINT stride = sizeof(EELine2DVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_lineVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->VSSetShader(s_lineVS, NULL, 0);
		//ID3D11ShaderResourceView *texture = m_lineTex.GetTexture();
		//deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetShader(s_linePS, NULL, 0);
		deviceContext->Draw(4, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELine2D::CreateLineVertexBuffer()
	{
		SAFE_RELEASE(m_lineVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EELine2DVertex) * 4;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_lineVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}