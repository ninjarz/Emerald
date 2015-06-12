#include "EEAverage.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEAverageC
	//----------------------------------------------------------------------------------------------------
	bool EEAverageC::s_isAverageCInitialized = false;
	ID3D11Buffer *EEAverageC::s_averageBuffer = nullptr;
	ID3D11ComputeShader *EEAverageC::s_averageCS = nullptr;

	//----------------------------------------------------------------------------------------------------
	bool EEAverageC::InitializeAverageC()
	{
		if (!s_isAverageCInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			// AverageBuffer
			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EEAverageBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_averageBuffer);
			if (FAILED(result))
				return false;

			ID3D10Blob *errorMessage = nullptr;
			ID3D10Blob *computeShaderBuffer = nullptr;
			// CS
			result = D3DX11CompileFromFileW(L"EEShader\\EEAverageShader.hlsl", NULL, NULL, "AverageCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}
			result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_averageCS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(computeShaderBuffer);
				return false;
			}

			s_isAverageCInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEAverageC::EEAverageC()
		:
		m_target(),
		m_backup()
	{
		InitializeAverageC();
	}

	//----------------------------------------------------------------------------------------------------
	EEAverageC::EEAverageC(const EETexture& _src)
		:
		m_target(1, 1),
		m_backup(_src)
	{
		InitializeAverageC();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAverageC::Update()
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
		deviceContext->CSSetShader(s_averageCS, NULL, 0);
		deviceContext->Dispatch(1, 1, 1);

		// clear
		ID3D11ShaderResourceView *nullSRV[1] = { nullptr };
		deviceContext->CSSetShaderResources(0, 1, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		EEBitmap result;
		m_target.GetBitmap(result);
		printf("%d %d %d %d ", result.GetData()[0], result.GetData()[1], result.GetData()[2], result.GetData()[3]);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAverageC::Render()
	{
		if (!EEEffect::Render())
			return false;

		return true;
	}
}