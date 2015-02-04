#pragma once
#ifndef _EE_CAMERA_H_
#define _EE_CAMERA_H_

#include <d3d11.h>
#include <map>
#include <utility>
#include "EEObject.h"
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EECameraMode
	//----------------------------------------------------------------------------------------------------
	enum EECameraMode
	{
		EE_CAMERA_FIRST = 1,
		EE_CAMERA_THIRD = 3,
	};

	//EEHCamera
	//----------------------------------------------------------------------------------------------------
	typedef int EEHCamera;

	//EECameraDesc
	//----------------------------------------------------------------------------------------------------
	struct EECameraDesc
	{
		FLOAT3	  position;
		FLOAT3	  right;
		FLOAT3	  up;
		FLOAT3	  look;
		FLOAT     distance;
	};

	//EECamera
	//----------------------------------------------------------------------------------------------------
	class EECamera
	{
		friend class EECameraSystem;

	protected:
		static int s_cameraCounter;

	public:
		EECamera();
		EECamera(const EECameraDesc& _desc);
		EECamera(const EECamera& _camera);
		~EECamera();

		void MoveLook(float _dist);
		void MoveRight(float _dist);
		void MoveUp(float _dist);
		void MoveY(float _dist);
		void MoveLook_XZ(float _dist);
		void RotateRight(float _radians);
		void RotateUp(float _radians);
		void RotateY(float _radians);
		void Zoom(float _dist);
		//void ArcballX(float radians);
		//void Zoom(float dist);

		void SetPosition(float _x, float _y, float _z);
		void SetPosition(const FLOAT3& _pos);
		void SetLens(float _fovY, float _aspectRatio, float _nearZ, float _farZ);

		EEHCamera GetHCamera() const;
		const FLOAT3& GetPosition() const;
		const FLOAT3& GetRight() const;
		const FLOAT3& GetUp() const;
		const FLOAT3& GetLook() const;
		const FLOAT3& GetLookAt() const;
		bool IsViewDirty() const;
		const MATRIX& GetViewMatrix();
		float GetFovY() const;
		float GetFovX() const;
		float GetAspectRatio() const;
		float GetNearZ() const;
		float GetFarZ() const;
		bool IsLensDirty() const;
		const MATRIX& GetProjectionMatrix();

	protected:
		EEHCamera m_handle;
		//view
		FLOAT3 m_position;
		FLOAT3 m_right;
		FLOAT3 m_up;
		FLOAT3 m_look;
		float m_distance;
		FLOAT3 m_lookAt;
		bool m_isViewDirty;
		MATRIX m_viewMatrix;
		//projection
		float m_fovY;
		float m_aspectRatio;
		float m_nearZ;
		float m_farZ;
		bool m_isLensDirty;
		MATRIX m_projectionMatrix;
	};

	//EECameraBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EECameraBufferDesc
	{
		MATRIX orthoLHMatrix;
		MATRIX perspectiveFovLHMatrix;
		MATRIX viewMatrix;
	};

	//EECameraSystem
	//----------------------------------------------------------------------------------------------------
	class EECameraSystem
	{
	public:
		EECameraSystem();
		EECameraSystem(const EECameraSystem& _system);
		~EECameraSystem();

		bool Initialize();
		void Shutdown();

		bool Process(EECameraMode _mode);
		bool MapCameraBuffer();
		EEHCamera CreateCamera(const EECameraDesc& _desc);
		bool DeleteCamera(EEHCamera _camera);

		void ClearCamera();
		bool SetCamera(EEHCamera _camera);
		EEHCamera GetCamera();
		const MATRIX& GetViewMatrix();
		const MATRIX& GetProjectionMatrix();
		const MATRIX& GetOrthoLHMatrix();

	protected:
		bool InitializeCameraBuffer();

	protected:
		ID3D11Buffer *m_cameraBuffer;
		bool m_isBufferDirty;
		//2D
		MATRIX m_orthoLHMatrix;
		//3D
		std::map<EEHCamera, EECamera*> m_cameras;
		EEHCamera m_currCamera;
	};

	//EECamera_APIs
	//----------------------------------------------------------------------------------------------------
	bool EECameraProcess(EECameraMode _mode);
	bool EEMapCameraBuffer();
	EEHCamera EECreateCamera(const EECameraDesc& _desc);
	bool EEDeleteCamera(EEHCamera _camera);
	void EEClearCamera();
	bool EESetCamera(EEHCamera _camera);
	EEHCamera EEGetCamera();
	const MATRIX& EEGetViewMatrix();
	const MATRIX& EEGetProjectionMatrix();
	const MATRIX& EEGetOrthoLHMatrix();
}

#endif