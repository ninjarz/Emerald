#include "EESphere.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESphere
	//----------------------------------------------------------------------------------------------------
	EESphere::EESphere(float _radius, int _slice, int _stack)
		:
		EEObject3D(),
		m_radius(_radius),
		m_slice(_slice),
		m_stack(_stack)
	{
		CreateSphereBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EESphere::EESphere(float _radius, int _slice, int _stack, const EETexture& _tex)
		:
		EEObject3D(),
		m_radius(_radius),
		m_slice(_slice),
		m_stack(_stack)
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateSphereBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EESphere::EESphere(const EESphere& _sphere)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESphere::~EESphere()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EESphere::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isScaleDirty)
		{
			CreateSphereBuffer();

			m_isScaleDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EESphere::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_objectIL);
		UINT stride = sizeof(EESphereVertex);
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
	bool EESphere::CreateSphereBuffer()
	{
		int perRow = m_slice + 1;
		int rowCount = m_stack - 1;

		m_vertexCount = perRow * rowCount + 2;
		std::vector<EESphereVertex> vertices(m_vertexCount);
		for (int i = 1; i <= rowCount; ++i)
		{
			float phy = EE_PI * i / m_stack;
			float tmpRadius = m_radius * sin(phy);
			for (int j = 0; j < perRow; ++j)
			{
				float theta = EE_2PI * j / m_slice;
				UINT index = (i - 1) * perRow + j;

				float x = tmpRadius * cos(theta);
				float y = m_radius * cos(phy);
				float z = tmpRadius * sin(theta);

				vertices[index].pos = FLOAT3(x, y, z);
				vertices[index].normal = FLOAT3(x, y, z).GetNormalization();
				vertices[index].tex = FLOAT2(j / (float)m_slice, i / (float)m_stack);
			}
		}
		vertices[vertices.size() - 2].pos = FLOAT3(0.f, m_radius, 0.f);
		vertices[vertices.size() - 2].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[vertices.size() - 2].tex = FLOAT2(0.f, 0.f);
		vertices[vertices.size() - 1].pos = FLOAT3(0.f, -m_radius, 0.f);
		vertices[vertices.size() - 1].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[vertices.size() - 1].tex = FLOAT2(0.f, 1.f);
		SAFE_RELEASE(m_objectVB);
		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EESphereVertex)* vertices.size();
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

		m_indexCount = (rowCount - 1) * m_slice * 6 + m_slice * 6;
		std::vector<UINT> indices(m_indexCount);
		UINT tmp = 0;
		int start1 = 0;
		int start2 = vertices.size() - perRow - 2;
		int top = vertices.size() - 2;
		int bottom = vertices.size() - 1;
		for (int i = 0; i < m_slice; ++i)
		{
			indices[tmp] = top;
			indices[tmp + 1] = start1 + i + 1;
			indices[tmp + 2] = start1 + i;

			tmp += 3;
		}
		for (int i = 0; i < m_slice; ++i)
		{
			indices[tmp] = bottom;
			indices[tmp + 1] = start2 + i;
			indices[tmp + 2] = start2 + i + 1;

			tmp += 3;
		}
		for (int i = 0; i < rowCount - 1; ++i)
		{
			for (int j = 0; j < m_slice; ++j)
			{
				indices[tmp] = i * perRow + j;
				indices[tmp + 1] = (i + 1) * perRow + j + 1;
				indices[tmp + 2] = (i + 1) * perRow + j;
				indices[tmp + 3] = i * perRow + j;
				indices[tmp + 4] = i * perRow + j + 1;
				indices[tmp + 5] = (i + 1) * perRow + j + 1;

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