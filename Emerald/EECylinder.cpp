#include "EECylinder.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EECylinder
	//----------------------------------------------------------------------------------------------------
	EECylinder::EECylinder(float _topRadius, float _bottomRadius, float _height, int _slice, int _stack)
		:
		EEObject3D(),
		m_topRadius(_topRadius),
		m_bottomRadius(_bottomRadius),
		m_height(_height),
		m_slice(_slice),
		m_stack(_stack)
	{
		CreateCylinderBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EECylinder::EECylinder(float _topRadius, float _bottomRadius, float _height, int _slice, int _stack, const EETexture& _tex)
		:
		EEObject3D(),
		m_topRadius(_topRadius),
		m_bottomRadius(_bottomRadius),
		m_height(_height),
		m_slice(_slice),
		m_stack(_stack)
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateCylinderBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EECylinder::EECylinder(const EECylinder& _cylinder)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EECylinder::~EECylinder()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EECylinder::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		if (m_isScaleDirty)
		{
			CreateCylinderBuffer();

			m_isScaleDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EECylinder::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_objectIL);
		UINT stride = sizeof(EECylinderVertex);
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
	bool EECylinder::CreateCylinderBuffer()
	{
		float radiusDelta = (m_bottomRadius - m_topRadius) / m_stack;
		float heightDelta = m_height / m_stack;
		int perRow = m_slice + 1;
		int rows = m_stack + 1;
		float topY = m_height * 0.5f;

		m_vertexCount = perRow * rows + 2 * (m_slice + 2);
		std::vector<EECylinderVertex> vertices(m_vertexCount);
		int index = 0;
		for (int i = 0; i < rows; ++i)
		{
			float tmpY = topY - heightDelta * i;
			float tmpRadius = m_topRadius + i * radiusDelta;
			for (int j = 0; j < perRow; ++j)
			{
				float theta = EE_2PI * j / m_slice;
				index = i * perRow + j;
				vertices[index].pos = FLOAT3(tmpRadius*cos(theta), tmpY, tmpRadius*sin(theta));
				vertices[index].normal = FLOAT3(cos(theta), (m_bottomRadius - m_topRadius) / m_height, sin(theta)).GetNormalization();
				vertices[index].tex = FLOAT2(1.f*j / m_slice, 1.f*i / m_stack);
			}
		}
		// top
		++index;
		for (int i = 0; i < m_slice + 1; ++i)
		{
			float theta = EE_2PI * i / m_slice;
			float x = m_topRadius * cosf(theta);
			float y = m_height * 0.5f;
			float z = m_topRadius * sinf(theta);
			float u = x / m_height + 0.5f;
			float v = z / m_height + 0.5f;
			vertices[index].pos = FLOAT3(x, y, z);
			vertices[index].normal = FLOAT3(0.f, 1.f, 0.f);
			vertices[index++].tex = FLOAT2(u, v);
		}
		vertices[index].pos = FLOAT3(0.f, m_height * 0.5f, 0.f);
		vertices[index].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[index++].tex = FLOAT2(0.5f, 0.5f);
		// bottom
		for (int i = 0; i < m_slice + 1; ++i)
		{
			float theta = EE_2PI * i / m_slice;
			float x = m_bottomRadius * cosf(theta);
			float y = -m_height * 0.5f;
			float z = m_bottomRadius * sinf(theta);
			float u = x / m_height + 0.5f;
			float v = z / m_height + 0.5f;
			vertices[index].pos = FLOAT3(x, y, z);
			vertices[index].normal = FLOAT3(0.f, -1.f, 0.f);
			vertices[index++].tex = FLOAT2(u, v);
		}
		vertices[index].pos = FLOAT3(0.f, -m_height * 0.5f, 0.f);
		vertices[index].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[index++].tex = FLOAT2(0.5f, 0.5f);
		SAFE_RELEASE(m_objectVB);
		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EECylinderVertex)* vertices.size();
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

		m_indexCount = m_slice * m_stack * 6 + m_slice * 6;
		std::vector<unsigned int> indices(m_indexCount);
		index = 0;
		for (int i = 0; i < m_stack; ++i)
		{
			for (int j = 0; j < m_slice; ++j)
			{
				indices[index] = i * perRow + j;
				indices[index + 1] = (i + 1) * perRow + j + 1;
				indices[index + 2] = (i + 1) * perRow + j;
				indices[index + 3] = i * perRow + j;
				indices[index + 4] = i * perRow + j + 1;
				indices[index + 5] = (i + 1) * perRow + j + 1;

				index += 6;
			}
		}
		// top
		int start = perRow * rows;
		int center = perRow * rows + m_slice + 1;
		for (int i = 0; i < m_slice; ++i)
		{
			indices[index++] = center;
			indices[index++] = start + i + 1;
			indices[index++] = start + i;
		}
		// bottom
		start = perRow * rows + m_slice + 2;
		center = perRow * rows + 2 * (m_slice + 2) - 1;
		for (int i = 0; i < m_slice; ++i)
		{
			indices[index++] = center;
			indices[index++] = start + i;
			indices[index++] = start + i + 1;
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
			MessageBoxW(NULL, L"CreateIndexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}