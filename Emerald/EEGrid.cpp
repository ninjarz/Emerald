#include "EEGrid.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEGrid
	//----------------------------------------------------------------------------------------------------
	EEGrid::EEGrid(float _width, float _height, unsigned int _m, unsigned int _n)
		:
		EEObject3D(),
		m_size(_width, _height),
		m_m(_m),
		m_n(_n)
	{
		CreateGridBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEGrid::EEGrid(float _width, float _height, unsigned int _m, unsigned int _n, const EETexture& _tex)
		:
		EEObject3D(),
		m_size(_width, _height),
		m_m(_m),
		m_n(_n)
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateGridBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEGrid::EEGrid(const EEGrid& _grid)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEGrid::~EEGrid()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEGrid::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isScaleDirty)
		{
			CreateGridBuffer();

			m_isScaleDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGrid::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_objectIL);
		UINT stride = sizeof(EEGridVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_objectVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(m_objectIB, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_objectVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_objectPS, NULL, 0);
		deviceConstext->DrawIndexed(m_indexCount, 0, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEGrid::CreateGridBuffer()
	{
		UINT rows = m_m + 1;
		UINT cols = m_n + 1;

		float oX = -m_size.x * 0.5f;
		float oZ = m_size.y * 0.5f;
		float dx = m_size.x / m_m;
		float dz = m_size.y / m_n;

		m_vertexCount = rows * cols;
		std::vector<EEGridVertex> vertices(m_vertexCount);
		for (UINT i = 0; i < cols; ++i)
		{
			float tmpZ = oZ - dz * i;
			for (UINT j = 0; j < rows; ++j)
			{
				UINT index = rows * i + j;
				vertices[index].pos.x = oX + dx * j;
				vertices[index].pos.y = 0.f;
				vertices[index].pos.z = tmpZ;

				vertices[index].normal = FLOAT3(0.f, 1.f, 0.f);
				vertices[index].tex = FLOAT2((float)i / m_m, (float)j / m_n);
			}
		}
		
		SAFE_RELEASE(m_objectVB);
		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEGridVertex)* vertices.size();
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

		m_indexCount = m_m * m_n * 6;
		std::vector<UINT> indices(m_indexCount);
		UINT tmp = 0;
		for (int i = 0; i < m_n; ++i)
		{
			for (int j = 0; j < m_m; ++j)
			{
				indices[tmp] = i * rows + j;
				indices[tmp + 1] = i * rows + j + 1;
				indices[tmp + 2] = (i + 1) * rows + j;
				indices[tmp + 3] = i * rows + j + 1;
				indices[tmp + 4] = (i + 1) * rows + j + 1;
				indices[tmp + 5] = (i + 1) * rows + j;

				tmp += 6;
			}
		}
		
		SAFE_RELEASE(m_objectIB);
		D3D11_BUFFER_DESC ibDesc = { 0 };
		ibDesc.ByteWidth = sizeof(UINT)* indices.size();
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;
		ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA ibData;
		ibData.pSysMem = &indices[0];
		ibData.SysMemPitch = 0;
		ibData.SysMemSlicePitch = 0;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&ibDesc, &ibData, &m_objectIB)))
		{
			MessageBox(NULL, L"CreateIndexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}