#include "EEGray.h"
#include "EECore.h"

#define GROUPDIX_X 32.f
#define GROUPDIX_Y 32.f

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEGrayC
	//----------------------------------------------------------------------------------------------------
	bool EEGrayC::s_isGrayCInitialized = false;
	ID3D11Buffer *EEGrayC::s_grayBuffer = nullptr;
	ID3D11ComputeShader *EEGrayC::s_grayCS = nullptr;

	//----------------------------------------------------------------------------------------------------
	bool EEGrayC::InitializeGrayC()
	{
		if (!s_isGrayCInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			// GrayBuffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EEGrayBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_grayBuffer);
			if (FAILED(result))
				return false;

			ID3D10Blob *errorMessage = nullptr;
			ID3D10Blob *computeShaderBuffer = nullptr;
			// CS
			result = D3DX11CompileFromFileW(L"EEShader\\EEGrayShader.hlsl", NULL, NULL, "GrayCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}
			result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_grayCS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}

			s_isGrayCInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEGrayC::EEGrayC(EETexture& _target)
		:
		m_target(_target),
		m_backup()
	{
		InitializeGrayC();

		if (m_target.GetTexture())
		{
			ID3D11Resource *resource = nullptr;
			m_target.GetTexture()->GetResource(&resource);
			D3D11_TEXTURE2D_DESC texture2DDesc;
			((ID3D11Texture2D*)resource)->GetDesc(&texture2DDesc);
			ID3D11Texture2D *resourceBackup = nullptr;
			if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateTexture2D(&texture2DDesc, NULL, &resourceBackup)))
			{
				m_backup.SetTexture(resourceBackup);
				EECore::s_EECore->GetDeviceContext()->CopyResource(resourceBackup, resource);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGrayC::Update()
	{
		if (!EEEffect::Update())
			return false;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		// set resource
		ID3D11ShaderResourceView *texture = m_backup.GetTexture();
		deviceContext->CSSetShaderResources(0, 1, &texture);
		ID3D11UnorderedAccessView *textureUAV = m_target.GetTextureUAV();
		deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, NULL);

		// dispatch
		deviceContext->CSSetShader(s_grayCS, NULL, 0);
		deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / GROUPDIX_X), (int)ceilf(m_target.GetHeight() / GROUPDIX_Y), 1);

		// clear
		ID3D11ShaderResourceView *nullSRV[1] = { nullptr };
		deviceContext->CSSetShaderResources(0, 1, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGrayC::Render()
	{
		if (!EEEffect::Render())
			return false;

		return true;
	}
}