#include "EECurve2D.h"
#include "EECore.h"
#include "EECollision.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EELine2D
	//----------------------------------------------------------------------------------------------------
	bool EECurve2D::s_isCurve2DInitialized = false;
	ID3D11InputLayout *EECurve2D::s_curveIL = NULL;
	ID3D11VertexShader *EECurve2D::s_curveVS = NULL;
	ID3D11PixelShader  *EECurve2D::s_curvePS = NULL;
	ID3D11Buffer *EECurve2D::s_curveBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EECurve2D::InitializeCurve2D()
	{
		if (!s_isCurve2DInitialized)
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
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_curveVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_curvePS);
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
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(quadInputDesc, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_curveIL);
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

			s_isCurve2DInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EECurve2D::EECurve2D()
		:
		EEObject2D(),
		m_curve(),
		m_width(1.f),
		m_isCurveDirty(true),
		m_curveVB(nullptr)
	{
		InitializeCurve2D();

	}

	EECurve2D::EECurve2D(const EETexture& _tex)
		:
		EEObject2D(),
		m_curve(),
		m_width(1.f),
		m_isCurveDirty(true),
		m_curveVB(nullptr)
	{
		InitializeCurve2D();
		SetIsUseColor(false);
		SetIsUseTex(true);
		SetTexture(_tex);
	}

	//----------------------------------------------------------------------------------------------------
	EECurve2D::EECurve2D(const std::vector<FLOAT2>& _curve, const EETexture& _tex)
		:
		EEObject2D(),
		m_curve(_curve),
		m_width(1.f),
		m_isCurveDirty(true),
		m_curveVB(nullptr)
	{
		InitializeCurve2D();
		SetIsUseColor(false);
		SetIsUseTex(true);
		SetTexture(_tex);

		CreateCurveVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EECurve2D::EECurve2D(const std::vector<FLOAT2>& _cruve, const EEColor& _color)
		:
		EEObject2D(),
		m_curve(_cruve),
		m_width(1.f),
		m_isCurveDirty(true),
		m_curveVB(nullptr)
	{
		InitializeCurve2D();
		SetColor(_color);

		CreateCurveVertexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EECurve2D::~EECurve2D()
	{
		SAFE_RELEASE(m_curveVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EECurve2D::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isLocalZOrderDirty || m_isCurveDirty)
		{
			std::vector<EECurve2DVertex> vertices(m_curve.size() << 1);
			if (m_curve.size() > 1)
			{
				float halfWidth = m_width / 2;
				float deltaTex = 1.f / m_curve.size();
				for (unsigned int i = 0; i < m_curve.size() - 1; ++i)
				{
					int index = i << 1;
					FLOAT2 vertical = (m_curve[i + 1] - m_curve[i]).GetVertical();
					vertices[index].pos = FLOAT3(m_curve[i] + vertical * halfWidth, m_localZOrder * 0.0001f);
					vertices[index].tex = FLOAT2(m_texRect.x, m_texRect.y + i * deltaTex);
					vertices[index + 1].pos = FLOAT3(m_curve[i] - vertical * halfWidth, m_localZOrder * 0.0001f);
					vertices[index + 1].tex = FLOAT2(m_texRect.z, m_texRect.y + i * deltaTex);
				}
				FLOAT2 vertical = (m_curve[m_curve.size() - 1] - m_curve[m_curve.size() - 2]).GetVertical();
				vertices[vertices.size() - 2].pos = FLOAT3(m_curve.back() + vertical * halfWidth, m_localZOrder * 0.0001f);
				vertices[vertices.size() - 2].tex = FLOAT2(m_texRect.x, m_texRect.w);
				vertices[vertices.size() - 1].pos = FLOAT3(m_curve.back() - vertical * halfWidth, m_localZOrder * 0.0001f);
				vertices[vertices.size() - 1].tex = FLOAT2(m_texRect.z, m_texRect.w);
			}

			if (m_isCurveDirty)
				CreateCurveVertexBuffer();
			if (m_curveVB)
			{
				ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
				deviceContext->Map(m_curveVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &vertices[0], sizeof(EECurve2DVertex)* vertices.size());
				deviceContext->Unmap(m_curveVB, 0);
			}

			m_isLocalZOrderDirty = false;
			m_isCurveDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECurve2D::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		deviceContext->IASetInputLayout(s_curveIL);
		UINT stride = sizeof(EECurve2DVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_curveVB, &stride, &offset);
		deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceContext->VSSetShader(s_curveVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceContext->PSSetShaderResources(0, 1, &texture);
		deviceContext->PSSetShader(s_curvePS, NULL, 0);
		deviceContext->Draw(m_curve.size() << 1, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECurve2D::SetCurve(const std::vector<FLOAT2>& _curve)
	{
		m_curve = _curve;
		m_isCurveDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECurve2D::SetWidth(float _width)
	{
		m_width = _width;
		m_isCurveDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECurve2D::CreateCurveVertexBuffer()
	{
		SAFE_RELEASE(m_curveVB);
		if (m_curve.size())
		{
			D3D11_BUFFER_DESC vbDesc = { 0 };
			vbDesc.ByteWidth = sizeof(EECurve2DVertex) * m_curve.size() << 1;
			vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			vbDesc.MiscFlags = 0;
			vbDesc.StructureByteStride = 0;
			vbDesc.Usage = D3D11_USAGE_DYNAMIC;
			if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_curveVB)))
			{
				MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
				return false;
			}
			return true;
		}

		return false;
	}
}