#include "EECamera.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EECamera
	//----------------------------------------------------------------------------------------------------
	bool EECamera::s_isCameraInitialized = false;
	ID3D11Buffer *EECamera::s_cameraBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EECamera::InitializeCameraBuffer()
	{
		if (!s_isCameraInitialized)
		{
			HRESULT result;
			ID3D11Device* device = EECore::s_EECore->GetDevice();
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

			EECameraBufferDesc cameraBufferDesc;
			cameraBufferDesc.orthoLH = MatrixOrthoLH(EECore::s_EECore->GetWidth(), EECore::s_EECore->GetHeight(), 0.0f, 1.0f);

			D3D11_BUFFER_DESC bufferDesc;
			bufferDesc.ByteWidth = sizeof(EECameraBufferDesc);
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bufferDesc.MiscFlags = 0;
			bufferDesc.StructureByteStride = 0;
			D3D11_SUBRESOURCE_DATA vbData;
			vbData.pSysMem = &cameraBufferDesc;
			vbData.SysMemPitch = 0;
			vbData.SysMemSlicePitch = 0;
			result = device->CreateBuffer(&bufferDesc, NULL, &s_cameraBuffer);
			if (FAILED(result))
				return false;

			deviceContext->PSSetConstantBuffers(1, 1, &s_cameraBuffer);

			s_isCameraInitialized = true;
		}

		return true;
	}
}