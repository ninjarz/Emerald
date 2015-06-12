#include "EEObject3D.h"
#include"EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEObject3D
	//----------------------------------------------------------------------------------------------------
	bool EEObject3D::s_isInitialized = false;
	ID3D11InputLayout *EEObject3D::s_objectIL = NULL;
	ID3D11VertexShader *EEObject3D::s_objectVS = NULL;
	ID3D11PixelShader  *EEObject3D::s_objectPS = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEObject3D::Initialize()
	{
		if (!s_isInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEBoxShader.hlsl", NULL, NULL, "BoxVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
			result = D3DX11CompileFromFileW(L"EEShader\\EEBoxShader.hlsl", NULL, NULL, "BoxPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_objectVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_objectPS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC boxInputDesc[3] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(boxInputDesc, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_objectIL);
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

			s_isInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEObject3D::EEObject3D()
		:
		EEObject(),
		m_objectVB(nullptr),
		m_vertexCount(0),
		m_objectIB(nullptr),
		m_indexCount(0),
		m_axisAlignedBox()
	{
		Initialize();
	}

	//----------------------------------------------------------------------------------------------------
	EEObject3D::EEObject3D(const FLOAT3& _position)
		:
		EEObject(_position),
		m_objectVB(nullptr),
		m_vertexCount(0),
		m_objectIB(nullptr),
		m_indexCount(0),
		m_axisAlignedBox()
	{
		Initialize();
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject3D::GetViewMatrix()
	{
		return EECore::s_EECore->GetViewMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	MATRIX EEObject3D::GetProjectionMatrix()
	{
		return EECore::s_EECore->GetProjectionMatrix();
	}

}