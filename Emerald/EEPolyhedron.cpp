#include "EEPolyhedron.h"
#include "EECore.h"
#include <fstream>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEPolyhedron
	//----------------------------------------------------------------------------------------------------
	EEPolyhedron::EEPolyhedron(const LPCWSTR _fileName)
		:
		EEObject3D()
	{
		CreatePolyhedronBuffer(_fileName);
	}

	//----------------------------------------------------------------------------------------------------
	EEPolyhedron::EEPolyhedron(const LPCWSTR _fileName, const EETexture& _tex)
		:
		EEObject3D()
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreatePolyhedronBuffer(_fileName);
	}

	//----------------------------------------------------------------------------------------------------
	EEPolyhedron::EEPolyhedron(const EEPolyhedron& _polyhedron)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEPolyhedron::~EEPolyhedron()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEPolyhedron::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isScaleDirty)
		{
			//CreatePolyhedronBuffer();

			m_isScaleDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEPolyhedron::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_objectIL);
		UINT stride = sizeof(EEPolyhedronVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_objectVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_objectVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_objectPS, NULL, 0);
		deviceConstext->Draw(m_vertexCount, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEPolyhedron::CreatePolyhedronBuffer(const LPCWSTR _fileName)
	{
		std::ifstream fin;
		fin.open(_fileName);
		if (fin.fail())
			return false;

		char temp;
		fin.get(temp);
		while (temp != ':')
		{
			fin.get(temp);
		}

		fin >> m_vertexCount;
		std::vector<EEPolyhedronVertex> vertices(m_vertexCount);
		for (int i = 0; i < m_vertexCount; ++i)
		{
			fin.get(temp);
			fin >> vertices[i].pos.x
				>> vertices[i].pos.y
				>> vertices[i].pos.z
				>> vertices[i].tex.x
				>> vertices[i].tex.y
				>> vertices[i].normal.x
				>> vertices[i].normal.y
				>> vertices[i].normal.z;
		}
		fin.close();
		SAFE_RELEASE(m_objectVB);
		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEPolyhedronVertex) * vertices.size();
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = &vertices[0];
		vbData.SysMemPitch = 0;
		vbData.SysMemSlicePitch = 0;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, &vbData, &m_objectVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}