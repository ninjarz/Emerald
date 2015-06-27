#include "EECanny.h"
#include "EECore.h"

#define GROUPDIX_X 32.f
#define GROUPDIX_Y 32.f

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EECannyC
	//----------------------------------------------------------------------------------------------------
	bool EECannyC::s_isCannyCInitialized = false;
	ID3D11Buffer *EECannyC::s_cannyBuffer = nullptr;
	ID3D11ComputeShader *EECannyC::s_cannyCS = nullptr;

	//----------------------------------------------------------------------------------------------------
	bool EECannyC::InitializeCannyC()
	{
		if (!s_isCannyCInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			// CannyBuffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EECannyBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_cannyBuffer);
			if (FAILED(result))
				return false;

			ID3D10Blob *errorMessage = nullptr;
			ID3D10Blob *computeShaderBuffer = nullptr;
			// CS
			result = D3DX11CompileFromFileW(L"EEShader\\EECannyShader.hlsl", NULL, NULL, "CannyCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}
			result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_cannyCS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}

			s_isCannyCInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EECannyC::EECannyC(EETexture& _target, EETexture& _gray, const INT2& _kernel)
		:
		m_backup(_target.Clone()),
		m_backupGray(_gray),
		m_gradient(_target.GetWidth(), _target.GetHeight()),
		m_target(_target),
		m_kernel(_kernel)
	{
		InitializeCannyC();
	}

	//----------------------------------------------------------------------------------------------------
	bool EECannyC::Update()
	{
		if (!EEEffect::Update())
			return false;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		// set cbuffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(deviceContext->Map(s_cannyBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;
		EECannyBufferDesc *buffer = (EECannyBufferDesc*)mappedResource.pData;
		buffer->kernel = m_kernel;
		deviceContext->Unmap(s_cannyBuffer, 0);
		deviceContext->CSSetConstantBuffers(3, 1, &s_cannyBuffer);

		// set resource
		ID3D11ShaderResourceView *texture[2] = { m_backup.GetTexture(), m_backupGray.GetTexture() };
		deviceContext->CSSetShaderResources(0, 2, texture);
		ID3D11UnorderedAccessView *textureUAV = m_target.GetTextureUAV();
		deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, NULL);

		// dispatch
		deviceContext->CSSetShader(s_cannyCS, NULL, 0);
		deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / GROUPDIX_X), (int)ceilf(m_target.GetHeight() / GROUPDIX_Y), 1);

		// clear
		ID3D11ShaderResourceView *nullSRV[2] = { nullptr, nullptr };
		deviceContext->CSSetShaderResources(0, 1, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECannyC::Render()
	{
		if (!EEEffect::Render())
			return false;

		return true;
	}
}