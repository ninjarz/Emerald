#pragma once
#ifndef _EE_CAMERA_H_
#define _EE_CAMERA_H_

#include <d3d11.h>
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EECameraBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EECameraBufferDesc
	{
		MATRIX orthoLH;
	};

	//EECamera
	//----------------------------------------------------------------------------------------------------
	class EECamera
	{
	public:
		static bool InitializeCameraBuffer();

	protected:
		static bool s_isCameraInitialized;
		static ID3D11Buffer *s_cameraBuffer;

	public:
		EECamera();
		EECamera(const EECamera& _camera);
		~EECamera();

		bool Initialize();
		void Shutdown();
	};
}

#endif