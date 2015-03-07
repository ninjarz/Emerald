#include "EECamera.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EECamera
	//----------------------------------------------------------------------------------------------------
	int EECamera::s_cameraCounter = -1;

	//----------------------------------------------------------------------------------------------------
	EECamera::EECamera()
		:
		m_handle(++s_cameraCounter),
		m_position(0),
		m_right(0),
		m_up(0),
		m_look(0),
		m_distance(0),
		m_lookAt(0),
		m_isViewDirty(true),
		m_viewMatrix(0),
		m_fovY(0),
		m_aspectRatio(0),
		m_nearZ(0),
		m_farZ(0),
		m_isLensDirty(true),
		m_projectionMatrix(0)
	{
		SetLens(EE_PI * 0.5, (float)EECore::s_EECore->GetWidth() / (float)EECore::s_EECore->GetHeight(), 0.1f, 1000.0f);
	}

	//----------------------------------------------------------------------------------------------------
	EECamera::EECamera(const EECameraDesc& _desc)
		:
		m_handle(++s_cameraCounter),
		m_position(_desc.position),
		m_right(_desc.right),
		m_up(_desc.up),
		m_look(_desc.look),
		m_distance(_desc.distance),
		m_lookAt(_desc.position + _desc.look * _desc.distance),
		m_isViewDirty(true),
		m_viewMatrix(0),
		m_fovY(0),
		m_aspectRatio(0),
		m_nearZ(0),
		m_farZ(0),
		m_isLensDirty(true),
		m_projectionMatrix(0)
	{
		SetLens(EE_PI * 0.5, (float)EECore::s_EECore->GetWidth() / (float)EECore::s_EECore->GetHeight(), 0.1f, 1000.0f);
	}

	//----------------------------------------------------------------------------------------------------
	EECamera::EECamera(const EECamera& _camera)
		:
		m_handle(_camera.m_handle),
		m_position(_camera.m_position),
		m_right(_camera.m_right),
		m_up(_camera.m_up),
		m_look(_camera.m_look),
		m_distance(_camera.m_distance),
		m_lookAt(_camera.m_distance),
		m_isViewDirty(_camera.m_isViewDirty),
		m_viewMatrix(_camera.m_viewMatrix),
		m_fovY(_camera.m_fovY),
		m_aspectRatio(_camera.m_aspectRatio),
		m_nearZ(_camera.m_nearZ),
		m_farZ(_camera.m_farZ),
		m_isLensDirty(_camera.m_isLensDirty),
		m_projectionMatrix(_camera.m_projectionMatrix)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EECamera::~EECamera()
	{

	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::MoveLook(float _dist)
	{
		m_position += m_look * _dist;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::MoveRight(float _dist)
	{
		m_position += m_right * _dist;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::MoveUp(float _dist)
	{
		m_position += m_up * _dist;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::MoveY(float _dist)
	{
		m_position += FLOAT3(0.f, 1.f, 0.f) * _dist;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::MoveLook_XZ(float _dist)
	{
		m_position += (m_look * FLOAT3(1.0f, 0.0f, 1.0f)).GetNormalization() * _dist;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::RotateRight(float _radians)
	{
		MATRIX rotation = MatrixRotationAxisN(m_right, _radians);
		(m_up *= rotation).Normalise();
		(m_look *= rotation).Normalise();

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::RotateUp(float _radians)
	{
		MATRIX rotation = MatrixRotationAxisN(m_up, _radians);
		(m_right *= rotation).Normalise();
		(m_look *= rotation).Normalise();

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::RotateY(float _radians)
	{
		MATRIX rotation = MatrixRotationAxisN(FLOAT3(0.0f, 1.0f, 0.0f), _radians);
		(m_up *= rotation).Normalise();
		(m_right *= rotation).Normalise();
		(m_look *= rotation).Normalise();

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::Zoom(float _dist)
	{
		m_distance -= (float)_dist;
		if (m_distance < 0)
			m_distance = 0;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::SetPosition(float _x, float _y, float _z)
	{
		m_position.x = _x;
		m_position.y = _y;
		m_position.z = _z;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::SetPosition(const FLOAT3& _pos)
	{
		m_position = _pos;

		m_isViewDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECamera::SetLens(float _fovY, float _aspectRatio, float _nearZ, float _farZ)
	{
		m_fovY = _fovY;
		m_aspectRatio = _aspectRatio;
		m_nearZ = _nearZ;
		m_farZ = _farZ;

		m_isLensDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	EEHCamera EECamera::GetHCamera() const
	{
		return m_handle;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EECamera::GetPosition() const
	{
		return m_position;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EECamera::GetRight() const
	{
		return m_right;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EECamera::GetUp() const
	{
		return m_up;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EECamera::GetLook() const
	{
		return m_look;
	}

	//----------------------------------------------------------------------------------------------------
	const FLOAT3& EECamera::GetLookAt() const
	{
		return m_lookAt;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECamera::IsViewDirty() const
	{
		return m_isViewDirty;
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EECamera::GetViewMatrix()
	{
		if (m_isViewDirty)
		{
			m_right = m_up.CrossProduct(m_look).GetNormalization();
			m_up = m_look.CrossProduct(m_right).GetNormalization();
			m_look = m_look.GetNormalization();
			FLOAT x = m_position.DotProduct(m_right);
			FLOAT y = m_position.DotProduct(m_up);
			FLOAT z = m_position.DotProduct(m_look);
			m_viewMatrix(0, 0) = m_right.x;	m_viewMatrix(0, 1) = m_up.x;	m_viewMatrix(0, 2) = m_look.x;	m_viewMatrix(0, 3) = 0;
			m_viewMatrix(1, 0) = m_right.y;	m_viewMatrix(1, 1) = m_up.y;	m_viewMatrix(1, 2) = m_look.y;	m_viewMatrix(1, 3) = 0;
			m_viewMatrix(2, 0) = m_right.z;	m_viewMatrix(2, 1) = m_up.z;	m_viewMatrix(2, 2) = m_look.z;	m_viewMatrix(2, 3) = 0;
			m_viewMatrix(3, 0) = -x;		m_viewMatrix(3, 1) = -y;		m_viewMatrix(3, 2) = -z;		m_viewMatrix(3, 3) = 1;

			m_isViewDirty = false;
		}

		return m_viewMatrix;
	}

	//----------------------------------------------------------------------------------------------------
	float EECamera::GetFovY() const
	{
		return m_fovY;
	}

	//----------------------------------------------------------------------------------------------------
	float EECamera::GetFovX() const
	{
		return atan(m_aspectRatio * tan(m_fovY * 0.5f)) * 2.f;
	}

	//----------------------------------------------------------------------------------------------------
	float EECamera::GetAspectRatio() const
	{
		return m_aspectRatio;
	}

	//----------------------------------------------------------------------------------------------------
	float EECamera::GetNearZ() const
	{
		return m_nearZ;
	}

	//----------------------------------------------------------------------------------------------------
	float EECamera::GetFarZ() const
	{
		return m_farZ;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECamera::IsLensDirty() const
	{
		return m_isLensDirty;
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EECamera::GetProjectionMatrix()
	{
		if (m_isLensDirty)
		{
			m_projectionMatrix = MatrixPerspectiveFovLH(m_fovY, m_aspectRatio, m_nearZ, m_farZ);

			m_isLensDirty = false;
		}

		return m_projectionMatrix;
	}

	//EECameraSystem
	//----------------------------------------------------------------------------------------------------
	EECameraSystem::EECameraSystem()
		:
		m_cameraBuffer(NULL),
		m_currCamera(0),
		m_isBufferDirty(true),
		m_orthoLHMatrix(MatrixOrthoLH((FLOAT)EECore::s_EECore->GetWidth(), (FLOAT)EECore::s_EECore->GetHeight(), 0.0f, 1.0f))
	{

	}

	//----------------------------------------------------------------------------------------------------
	EECameraSystem::EECameraSystem(const EECameraSystem& _system)
		:
		m_cameraBuffer(_system.m_cameraBuffer),
		m_currCamera(_system.m_currCamera)
	{

	}

	//----------------------------------------------------------------------------------------------------

	bool EECameraSystem::Initialize()
	{
		ClearCamera();
		InitializeCameraBuffer();
		MapCameraBuffer();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECameraSystem::Shutdown()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EECameraSystem::Process(EECameraMode _mode)
	{
		float speed = 50.f;  //tmp
		float inclineSpeed = 50.f * (float)sqrt(2) / 2;
		if (EECore::s_EECore->IsKeyDown('W'))
		{
			if (EECore::s_EECore->IsKeyDown('A'))
			{
				m_cameras[m_currCamera].MoveLook_XZ(inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
				m_cameras[m_currCamera].MoveRight(-inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
			}
			else if (EECore::s_EECore->IsKeyDown('D'))
			{
				m_cameras[m_currCamera].MoveLook_XZ(inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
				m_cameras[m_currCamera].MoveRight(inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
			}
			else
				m_cameras[m_currCamera].MoveLook_XZ(speed * (float)EECore::s_EECore->GetDeltaTime());
		}
		else if (EECore::s_EECore->IsKeyDown('S'))
		{
			if (EECore::s_EECore->IsKeyDown('A'))
			{
				m_cameras[m_currCamera].MoveLook_XZ(-inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
				m_cameras[m_currCamera].MoveRight(-inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
			}
			else if (EECore::s_EECore->IsKeyDown('D'))
			{
				m_cameras[m_currCamera].MoveLook_XZ(-inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
				m_cameras[m_currCamera].MoveRight(inclineSpeed * (float)EECore::s_EECore->GetDeltaTime());
			}
			else
				m_cameras[m_currCamera].MoveLook_XZ(-speed * (float)EECore::s_EECore->GetDeltaTime());
		}
		else if (EECore::s_EECore->IsKeyDown('A'))
		{
			m_cameras[m_currCamera].MoveRight(-speed * (float)EECore::s_EECore->GetDeltaTime());
		}
		else if (EECore::s_EECore->IsKeyDown('D'))
		{
			m_cameras[m_currCamera].MoveRight(speed * (float)EECore::s_EECore->GetDeltaTime());
		}

		if (EECore::s_EECore->IsKeyDown(VK_SPACE))
		{
			m_cameras[m_currCamera].MoveY(speed * (float)EECore::s_EECore->GetDeltaTime());
		}
		else if (EECore::s_EECore->IsKeyDown('C'))
		{
			m_cameras[m_currCamera].MoveY(-speed * (float)EECore::s_EECore->GetDeltaTime());
		}

		switch (_mode)
		{
		case EE_CAMERA_FIRST:
			if (EECore::s_EECore->IsKeyDown(VK_LBUTTON))
			{
				m_cameras[m_currCamera].RotateY(EEDegreesToRadians(0.2f * (float)EECore::s_EECore->GetMouseDeltaX()));
				m_cameras[m_currCamera].RotateRight(EEDegreesToRadians(0.2f * (float)EECore::s_EECore->GetMouseDeltaY()));
			}
			break;
		case EE_CAMERA_THIRD:
			if (EECore::s_EECore->IsKeyDown(VK_LBUTTON))
			{
				//m_cameras[m_currCamera]->SetPosition();
				m_cameras[m_currCamera].RotateY(EEDegreesToRadians(0.2f * (float)EECore::s_EECore->GetMouseDeltaX()));
				m_cameras[m_currCamera].RotateRight(EEDegreesToRadians(0.2f * (float)EECore::s_EECore->GetMouseDeltaY()));
				m_cameras[m_currCamera].RotateRight(EEDegreesToRadians((float)EECore::s_EECore->GetMouseDeltaM()) / 120 * 8);
			}
			break;

		default:
			break;
		}
		
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECameraSystem::MapCameraBuffer()
	{
		if (m_isBufferDirty || m_cameras[m_currCamera].IsViewDirty() || m_cameras[m_currCamera].IsLensDirty())
		{
			HRESULT result;
			ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			EECameraBufferDesc *cameraBufferDesc;
			result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(result))
				return false;
			cameraBufferDesc = (EECameraBufferDesc*)mappedResource.pData;
			cameraBufferDesc->orthoLHMatrix = m_orthoLHMatrix;
			cameraBufferDesc->perspectiveFovLHMatrix = m_cameras[m_currCamera].GetProjectionMatrix();
			cameraBufferDesc->viewMatrix = m_cameras[m_currCamera].GetViewMatrix();

			deviceContext->VSSetConstantBuffers(1, 1, &m_cameraBuffer);

			m_isBufferDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEHCamera EECameraSystem::CreateCamera(const EECameraDesc& _desc)
	{
		EECamera camera(_desc);
		m_cameras.insert(std::pair<EEHCamera, EECamera>(camera.GetHCamera(), camera));
		return camera.GetHCamera();
	}

	//----------------------------------------------------------------------------------------------------
	bool EECameraSystem::DeleteCamera(EEHCamera _camera)
	{
		m_cameras.erase(_camera);
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EECameraSystem::ClearCamera()
	{
		EECamera::s_cameraCounter = -1;
		m_cameras.clear();

		EECameraDesc desc;
		desc.position = FLOAT3(0.0f, 0.0f, -30.0f);
		desc.right = FLOAT3(1.0f, 0.0f, 0.0f);
		desc.up = FLOAT3(0.0f, 1.0f, 0.0f);
		desc.look = FLOAT3(0.0f, 0.0f, 1.0f);
		desc.distance = 1.0f;
		EECamera camera(desc);
		m_cameras.insert(std::pair<EEHCamera, EECamera>(camera.GetHCamera(), camera));
		m_currCamera = camera.GetHCamera();

		m_isBufferDirty = true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECameraSystem::SetCamera(EEHCamera _camera)
	{
		if (m_cameras.find(_camera) != m_cameras.end())
		{
			m_currCamera = _camera;
			m_isBufferDirty = true;

			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	EEHCamera EECameraSystem::GetCamera()
	{
		return m_currCamera;
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EECameraSystem::GetViewMatrix()
	{
		return m_cameras[m_currCamera].GetViewMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EECameraSystem::GetProjectionMatrix()
	{
		return m_cameras[m_currCamera].GetProjectionMatrix();
	}

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EECameraSystem::GetOrthoLHMatrix()
	{
		return m_orthoLHMatrix;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECameraSystem::InitializeCameraBuffer()
	{
		HRESULT result;
		ID3D11Device* device = EECore::s_EECore->GetDevice();
		ID3D11DeviceContext* deviceContext = EECore::s_EECore->GetDeviceContext();

		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.ByteWidth = sizeof(EECameraBufferDesc);
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		result = device->CreateBuffer(&bufferDesc, NULL, &m_cameraBuffer);
		if (FAILED(result))
			return false;

		return true;
	}

	//EECamera_APIs
	//----------------------------------------------------------------------------------------------------
	bool EECameraProcess(EECameraMode _mode) { return EECore::s_EECore->GetEECameraSystem()->Process(_mode); }

	//----------------------------------------------------------------------------------------------------
	bool EEMapCameraBuffer() { return EECore::s_EECore->GetEECameraSystem()->MapCameraBuffer(); }

	//----------------------------------------------------------------------------------------------------
	EEHCamera EECreateCamera(const EECameraDesc& _desc) { return EECore::s_EECore->GetEECameraSystem()->CreateCamera(_desc); }

	//----------------------------------------------------------------------------------------------------
	bool EEDeleteCamera(EEHCamera _camera) { return EECore::s_EECore->GetEECameraSystem()->DeleteCamera(_camera); }

	//----------------------------------------------------------------------------------------------------
	void EEClearCamera() { return EECore::s_EECore->GetEECameraSystem()->ClearCamera(); }

	//----------------------------------------------------------------------------------------------------
	bool EESetCamera(EEHCamera _camera) { return EECore::s_EECore->GetEECameraSystem()->SetCamera(_camera); }

	//----------------------------------------------------------------------------------------------------
	EEHCamera EEGetCamera() { return EECore::s_EECore->GetEECameraSystem()->GetCamera(); }

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EEGetViewMatrix() { return EECore::s_EECore->GetEECameraSystem()->GetViewMatrix(); }

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EEGetProjectionMatrix() { return EECore::s_EECore->GetEECameraSystem()->GetProjectionMatrix(); }

	//----------------------------------------------------------------------------------------------------
	const MATRIX& EEGetOrthoLHMatrix() { return EECore::s_EECore->GetEECameraSystem()->GetOrthoLHMatrix(); }
}