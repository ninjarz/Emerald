#include "EERipple.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EERippleC
	//----------------------------------------------------------------------------------------------------
	bool EERippleC::s_isRippleCInitialized = false;
	ID3D11Buffer *EERippleC::s_disturbBuffer = nullptr;
	ID3D11Buffer *EERippleC::s_spreadBuffer = nullptr;
	ID3D11Buffer *EERippleC::s_rippleBuffer = nullptr;
	ID3D11ComputeShader *EERippleC::s_disturbCS = nullptr;
	ID3D11ComputeShader *EERippleC::s_spreadCS = nullptr;
	ID3D11ComputeShader *EERippleC::s_rippleCS = nullptr;

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::InitializeRippleC()
	{
		 if (!s_isRippleCInitialized)
		 {
			 HRESULT result;
			 ID3D11Device* device = EECore::s_EECore->GetDevice();
			 ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			 //DisturbBuffer
			 D3D11_BUFFER_DESC bufferDesc;
			 bufferDesc.ByteWidth = sizeof(EEDisturbBufferDesc);
			 bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			 bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			 bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			 bufferDesc.MiscFlags = 0;
			 bufferDesc.StructureByteStride = 0;
			 result = device->CreateBuffer(&bufferDesc, NULL, &s_disturbBuffer);
			 if (FAILED(result))
				 return false;
			 //SpreadBuffer
			 bufferDesc.ByteWidth = sizeof(EESpreadBufferDesc);
			 result = device->CreateBuffer(&bufferDesc, NULL, &s_spreadBuffer);
			 if (FAILED(result))
				 return false;
			 //RippleBuffer
			 bufferDesc.ByteWidth = sizeof(EERippleBufferDesc);
			 result = device->CreateBuffer(&bufferDesc, NULL, &s_rippleBuffer);
			 if (FAILED(result))
				 return false;

			 ID3D10Blob *errorMessage = nullptr;
			 ID3D10Blob *computeShaderBuffer = nullptr;
			 //DisturbCS
			 result = D3DX11CompileFromFileW(L"EEShader\\EERippleDisturb.hlsl", NULL, NULL, "DisturbCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			 if (FAILED(result))
			 {
				 if (errorMessage)
					 MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }
			 result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_disturbCS);
			 if (FAILED(result))
			 {
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }
			 //SpreadCS
			 result = D3DX11CompileFromFileW(L"EEShader\\EERippleSpread.hlsl", NULL, NULL, "SpreadCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			 if (FAILED(result))
			 {
				 if (errorMessage)
					 MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }
			 result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_spreadCS);
			 if (FAILED(result))
			 {
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }
			 //RippleCS
			 result = D3DX11CompileFromFileW(L"EEShader\\EERippleShader.hlsl", NULL, NULL, "RippleCS", "cs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &computeShaderBuffer, &errorMessage, NULL);
			 if (FAILED(result))
			 {
				 if (errorMessage)
					 MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "Compile Shader Error!", MB_OK);
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }
			 result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &s_rippleCS);
			 if (FAILED(result))
			 {
				 SAFE_RELEASE(errorMessage);
				 SAFE_RELEASE(computeShaderBuffer);
				 return false;
			 }

			 s_isRippleCInitialized = true;
		 }

		 return true;
	}

	//----------------------------------------------------------------------------------------------------
	EERippleC::EERippleC()
		:
		EEEffect(),
		m_target(),
		m_backup(),
		m_weightMap(),
		m_currWeightMap(0),
		m_spreadFactor(1),
		m_fadeFactor(10),
		m_refractiveIndex(8192.f),
		m_updateTime((float)EECore::s_EECore->GetTotalTime()),
		m_spreadInterval(0.f)
	{
		InitializeRippleC();

		/*
		ID3D11Resource *resource(nullptr);
		EECore::s_EECore->GetRenderTargetView()->GetResource(&resource);
		if (resource)
		{
			m_target.SetTexture(resource);
			D3D11_TEXTURE2D_DESC texture2DDesc;
			((ID3D11Texture2D*)resource)->GetDesc(&texture2DDesc);
			ID3D11Texture2D *resourceBackup(nullptr);
			if (SUCCEEDED(EECore::s_EECore->GetDevice()->CreateTexture2D(&texture2DDesc, NULL, &resourceBackup)))
			{
				m_backup.SetTexture(resourceBackup);
				EECore::s_EECore->GetDeviceContext()->CopyResource(resourceBackup, resource);
				m_weightMap[0].SetTexture(m_target.GetWidth(), m_target.GetHeight(), DXGI_FORMAT_R32_SINT);
				m_weightMap[1].SetTexture(m_target.GetWidth(), m_target.GetHeight(), DXGI_FORMAT_R32_SINT);
			}
		}
		*/
	}

	//----------------------------------------------------------------------------------------------------
	EERippleC::EERippleC(EETexture& _target)
		:
		EEEffect(),
		m_target(_target),
		m_backup(),
		m_weightMap(),
		m_currWeightMap(0),
		m_spreadFactor(1),
		m_fadeFactor(10),
		m_refractiveIndex(8192.f),
		m_updateTime((float)EECore::s_EECore->GetTotalTime()),
		m_spreadInterval(0.f)
	{
		InitializeRippleC();

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
				m_weightMap[0].SetTexture(m_target.GetWidth(), m_target.GetHeight(), DXGI_FORMAT_R32_SINT);
				m_weightMap[1].SetTexture(m_target.GetWidth(), m_target.GetHeight(), DXGI_FORMAT_R32_SINT);
			}
		}
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::Update()
	{
		if (!EEEffect::Update())
			return false;

		//Bad efficiency... Need to find the equation
		int loopTimes = m_spreadInterval == 0.0f ? 1 : (int)((EECore::s_EECore->GetTotalTime() - m_updateTime) / m_spreadInterval);
		int halfFPS = EECore::s_EECore->GetFPS() >> 1;
		if (loopTimes >= halfFPS)
			loopTimes = halfFPS + 1;
		m_updateTime += m_spreadInterval * loopTimes;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
		while (loopTimes--)
		{
			//Set cbuffer
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			if (FAILED(deviceContext->Map(s_spreadBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
				return false;
			EESpreadBufferDesc *buffer = (EESpreadBufferDesc*)mappedResource.pData;
			buffer->spreadFactor = m_spreadFactor;
			buffer->fadeFactor = m_fadeFactor;
			deviceContext->Unmap(s_spreadBuffer, 0);
			deviceContext->CSSetConstantBuffers(3, 1, &s_spreadBuffer);

			//Set resource
			ID3D11ShaderResourceView *texture = m_weightMap[m_currWeightMap++ % 2].GetTexture();
			deviceContext->CSSetShaderResources(0, 1, &texture);
			ID3D11UnorderedAccessView *textureUAV = m_weightMap[m_currWeightMap % 2].GetTextureUAV();
			deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, NULL);

			//Dispatch
			deviceContext->CSSetShader(s_spreadCS, NULL, 0);
			deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / 32.f), (int)ceilf(m_target.GetHeight() / 32.f), 1);

			//Clear
			ID3D11ShaderResourceView *nullSRV[1] = { nullptr };
			deviceContext->CSSetShaderResources(0, 1, nullSRV);
			ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
			deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::Render()
	{
		if (!EEEffect::Render())
			return false;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		//Set cbuffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(deviceContext->Map(s_rippleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;
		EERippleBufferDesc *buffer = (EERippleBufferDesc*)mappedResource.pData;
		buffer->refractiveIndex = m_refractiveIndex;
		deviceContext->Unmap(s_rippleBuffer, 0);
		deviceContext->CSSetConstantBuffers(3, 1, &s_rippleBuffer);

		//Set resource
		ID3D11ShaderResourceView *texture[2] = { m_weightMap[m_currWeightMap % 2].GetTexture(), m_backup.GetTexture() };
		deviceContext->CSSetShaderResources(0, 2, texture);
		ID3D11UnorderedAccessView *textureUAV = m_target.GetTextureUAV();
		deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, nullptr);

		//Dispatch
		deviceContext->CSSetShader(s_rippleCS, NULL, 0);
		deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / 32.f), (int)ceilf(m_target.GetHeight() / 32.f), 1);

		//Clear
		ID3D11ShaderResourceView *nullSRV[2] = { nullptr, nullptr };
		deviceContext->CSSetShaderResources(0, 2, nullSRV);
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::Disturb(int _x, int _y, int _range, int _weight)
	{
		if (!m_isAlive || !m_target.GetTexture())
			return false;

		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		//Set cbuffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (FAILED(deviceContext->Map(s_disturbBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
			return false;
		EEDisturbBufferDesc *buffer = (EEDisturbBufferDesc*)mappedResource.pData;
		buffer->pos.x = _x;
		buffer->pos.y = _y;
		buffer->range = _range;
		buffer->weight = _weight;
		deviceContext->Unmap(s_disturbBuffer, 0);
		deviceContext->CSSetConstantBuffers(3, 1, &s_disturbBuffer);

		//Set resource
		ID3D11UnorderedAccessView *textureUAV = m_weightMap[m_currWeightMap % 2].GetTextureUAV();
		deviceContext->CSSetUnorderedAccessViews(0, 1, &textureUAV, NULL);

		//Dispatch
		deviceContext->CSSetShader(s_disturbCS, NULL, 0);
		deviceContext->Dispatch((int)ceilf(m_target.GetWidth() / 32.f), (int)ceilf(m_target.GetHeight() / 32.f), 1);

		//Clear
		ID3D11UnorderedAccessView *nullUAV[1] = { nullptr };
		deviceContext->CSSetUnorderedAccessViews(0, 1, nullUAV, nullptr);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::SetSpreadFactor(int _factor)
	{
		m_spreadFactor = _factor;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::SetFadeFactor(int _factor)
	{
		m_fadeFactor = _factor;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EERippleC::SetRefractiveIndex(float _index)
	{
		m_refractiveIndex = _index;

		return true;
	}
}