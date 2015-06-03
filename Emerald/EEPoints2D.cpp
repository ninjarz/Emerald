#include "EEPoints2D.h"
#include "EECore.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEPoints2D
	//----------------------------------------------------------------------------------------------------
	bool EEPoints2D::s_isPoints2DInitialized = false;
	ID3D11InputLayout *EEPoints2D::s_pointsIL = NULL;
	ID3D11VertexShader *EEPoints2D::s_pointsVS = NULL;
	ID3D11PixelShader  *EEPoints2D::s_pointsPS = NULL;
	ID3D11Buffer *EEPoints2D::s_points2DBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEPoints2D::InitializePoints2D()
	{
		if (!s_isPoints2DInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEPoints2DShader.hlsl", NULL, NULL, "PointsVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
			result = D3DX11CompileFromFileW(L"EEShader\\EEPoints2DShader.hlsl", NULL, NULL, "PointsPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_pointsVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_pointsPS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC inputDesc[1] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(inputDesc, 1, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_pointsIL);
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

			s_isPoints2DInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEPoints2D::EEPoints2D()
		:
		EEObject2D(),
		m_isPointsDirty(true),
		m_points(),
		m_pointsVB(nullptr)
	{
		InitializePoints2D();
	}

	//----------------------------------------------------------------------------------------------------
	EEPoints2D::EEPoints2D(std::vector<FLOAT2>& _points)
		:
		EEObject2D(),
		m_isPointsDirty(true),
		m_points(_points),
		m_pointsVB(nullptr)
	{
		InitializePoints2D();
	}

	//----------------------------------------------------------------------------------------------------
	EEPoints2D::~EEPoints2D()
	{
		SAFE_RELEASE(m_pointsVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEPoints2D::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty || m_isPointsDirty)
		{
			std::vector<EEPoints2DVertex> vertices(m_points.size());
			for (unsigned int i = 0; i < m_points.size(); ++i)
			{
				vertices[i].pos = FLOAT3(m_points[i], m_localZOrder * 0.0001f);
			}

			if (m_isPointsDirty)
				CreatePointsVertexBuffer();
			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_pointsVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, &vertices[0], vertices.size() * sizeof(EEPoints2DVertex));
			deviceContext->Unmap(m_pointsVB, 0);

			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
			m_isPointsDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEPoints2D::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		deviceContext->IASetInputLayout(s_pointsIL);
		UINT stride = sizeof(EEPoints2DVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_pointsVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->VSSetShader(s_pointsVS, NULL, 0);
		deviceContext->PSSetShader(s_pointsPS, NULL, 0);
		deviceContext->Draw(m_points.size(), 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEPoints2D::AddPoints(std::vector<FLOAT2>& _points)
	{
		for (FLOAT2& point : _points)
		{
			m_points.push_back(point);
		}
		m_isPointsDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEPoints2D::AddPoints(std::vector<INT2>& _points)
	{
		for (INT2& point : _points)
		{
			m_points.push_back(FLOAT2((float)point.x, (float)point.y));
		}
		m_isPointsDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEPoints2D::CreatePointsVertexBuffer()
	{
		SAFE_RELEASE(m_pointsVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEPoints2DVertex) * m_points.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_pointsVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}