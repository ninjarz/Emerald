#include "EEDilate.h"
#include "EECore.h"

#define GROUPDIX_X 32.f
#define GROUPDIX_Y 32.f

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEDilateC
	//----------------------------------------------------------------------------------------------------
	bool EEDilateC::s_isDilateCInitialized = false;
	ID3D11Buffer *EEDilateC::s_dilateBuffer = nullptr;
	ID3D11ComputeShader *EEDilateC::s_dilateCS = nullptr;

	//----------------------------------------------------------------------------------------------------
	bool EEDilateC::InitializeDilateC()
	{
		if (!s_isDilateCInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			// DilateBuffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EEDilateBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_dilateBuffer);
			if (FAILED(result))
				return false;

			ID3D10Blob *errorMessage = nullptr;
			ID3D10Blob *computeShaderBuffer = nullptr;
			// CS
			result = D3DX11CompileFromFileW(L"EEShader\\EEDilateShader.hlsl", NULL, NULL, "DilateCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}
			result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_dilateCS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}

			s_isDilateCInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEDilateC::EEDilateC(EETexture& _target, EETexture& _gray, const INT2& _kernel)
		:
		m_backup(_target.Clone()),
		m_backupGray(_gray),
		m_target(_target),
		m_kernel(_kernel)
	{
		InitializeDilateC();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEDilateC::Update()
	{
		if (!EEEffect::Update())
			return false;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		// set cbuffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(deviceContext->Map(s_dilateBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;
		EEDilateBufferDesc *buffer = (EEDilateBufferDesc*)mappedResource.pData;
		buffer->kernel = m_kernel;
		deviceContext->Unmap(s_dilateBuffer, 0);
		deviceContext->CSSetConstantBuffers(3, 1, &s_dilateBuffer);

		// set resource
		ID3D11ShaderResourceView *texture[2] = { m_backup.GetTexture(), m_backupGray.GetTexture() };
		deviceContext->CSSetShaderResources(0, 2, texture);
		ID3D11UnorderedAccessView *textureUAV = m_target.GetTextureUAV();
		deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, NULL);

		// dispatch
		deviceContext->CSSetShader(s_dilateCS, NULL, 0);
		deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / GROUPDIX_X), (int)ceilf(m_target.GetHeight() / GROUPDIX_Y), 1);

		// clear
		ID3D11ShaderResourceView *nullSRV[2] = { nullptr, nullptr };
		deviceContext->CSSetShaderResources(0, 1, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEDilateC::Render()
	{
		if (!EEEffect::Render())
			return false;

		return true;
	}
}