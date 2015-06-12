#include "EEBox.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBox
	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, const FLOAT3& _size)
		:
		EEObject3D(_pos),
		m_size(_size)
	{
		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, const FLOAT3& _size, const EETexture& _tex)
		:
		EEObject3D(_pos),
		m_size(_size)
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, FLOAT _width, FLOAT _height, FLOAT _depth)
		:
		EEObject3D(_pos),
		m_size(_width, _height, _depth)
	{
		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, FLOAT _width, FLOAT _height, FLOAT _depth, const EETexture& _tex)
		:
		EEObject3D(_pos),
		m_size(_width, _height, _depth)
	{
		SetTexture(_tex);
		SetIsUseColor(false);
		SetIsUseTex(true);

		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const EEBox& _box)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBox::~EEBox()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEBox::Update()
	{
		if (!EEObject::Update())
			return false;

		UpdateObjectState();

		/*
		if (m_isSizeDirty)
		{
			CreateBoxVertexBuffer();

			m_isSizeDirty = false;
		}
		*/

		return true;
	}

	 //----------------------------------------------------------------------------------------------------
	bool EEBox::Render()
	{
		if (!EEObject::Render())
			return false;

		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_objectIL);
		UINT stride = sizeof(EEBoxVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_objectVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(m_objectIB, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_objectVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_objectPS, NULL, 0);
		deviceConstext->DrawIndexed(36, 0, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBox::CreateBoxVertexBuffer()
	{
		SAFE_RELEASE(m_objectVB);

		EEBoxVertex vertices[24];
		FLOAT halfW = m_size.x * 0.5f;
		FLOAT halfH = m_size.y * 0.5f;
		FLOAT halfD = m_size.z * 0.5f;

		vertices[0].pos = FLOAT3(-halfW, -halfH, -halfD);
		vertices[0].normal = FLOAT3(0.f, 0.f, -1.f);
		vertices[0].tex = FLOAT2(0.f, 1.f);
		vertices[1].pos = FLOAT3(-halfW, halfH, -halfD);
		vertices[1].normal = FLOAT3(0.f, 0.f, -1.f);
		vertices[1].tex = FLOAT2(0.f, 0.f);
		vertices[2].pos = FLOAT3(halfW, halfH, -halfD);
		vertices[2].normal = FLOAT3(0.f, 0.f, -1.f);
		vertices[2].tex = FLOAT2(1.f, 0.f);
		vertices[3].pos = FLOAT3(halfW, -halfH, -halfD);
		vertices[3].normal = FLOAT3(0.f, 0.f, -1.f);
		vertices[3].tex = FLOAT2(1.f, 1.f);

		vertices[4].pos = FLOAT3(-halfW, -halfH, halfD);
		vertices[4].normal = FLOAT3(-1.f, 0.f, 0.f);
		vertices[4].tex = FLOAT2(0.f, 1.f);
		vertices[5].pos = FLOAT3(-halfW, halfH, halfD);
		vertices[5].normal = FLOAT3(-1.f, 0.f, 0.f);
		vertices[5].tex = FLOAT2(0.f, 0.f);
		vertices[6].pos = FLOAT3(-halfW, halfH, -halfD);
		vertices[6].normal = FLOAT3(-1.f, 0.f, 0.f);
		vertices[6].tex = FLOAT2(1.f, 0.f);
		vertices[7].pos = FLOAT3(-halfW, -halfH, -halfD);
		vertices[7].normal = FLOAT3(-1.f, 0.f, 0.f);
		vertices[7].tex = FLOAT2(1.f, 1.f);

		vertices[8].pos = FLOAT3(halfW, -halfH, halfD);
		vertices[8].normal = FLOAT3(0.f, 0.f, 1.f);
		vertices[8].tex = FLOAT2(0.f, 1.f);
		vertices[9].pos = FLOAT3(halfW, halfH, halfD);
		vertices[9].normal = FLOAT3(0.f, 0.f, 1.f);
		vertices[9].tex = FLOAT2(0.f, 0.f);
		vertices[10].pos = FLOAT3(-halfW, halfH, halfD);
		vertices[10].normal = FLOAT3(0.f, 0.f, 1.f);
		vertices[10].tex = FLOAT2(1.f, 0.f);
		vertices[11].pos = FLOAT3(-halfW, -halfH, halfD);
		vertices[11].normal = FLOAT3(0.f, 0.f, 1.f);
		vertices[11].tex = FLOAT2(1.f, 1.f);

		vertices[12].pos = FLOAT3(halfW, -halfH, -halfD);
		vertices[12].normal = FLOAT3(1.f, 0.f, 0.f);
		vertices[12].tex = FLOAT2(0.f, 1.f);
		vertices[13].pos = FLOAT3(halfW, halfH, -halfD);
		vertices[13].normal = FLOAT3(1.f, 0.f, 0.f);
		vertices[13].tex = FLOAT2(0.f, 0.f);
		vertices[14].pos = FLOAT3(halfW, halfH, halfD);
		vertices[14].normal = FLOAT3(1.f, 0.f, 0.f);
		vertices[14].tex = FLOAT2(1.f, 0.f);
		vertices[15].pos = FLOAT3(halfW, -halfH, halfD);
		vertices[15].normal = FLOAT3(1.f, 0.f, 0.f);
		vertices[15].tex = FLOAT2(1.f, 1.f);

		vertices[16].pos = FLOAT3(-halfW, halfH, -halfD);
		vertices[16].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[16].tex = FLOAT2(0.f, 1.f);
		vertices[17].pos = FLOAT3(-halfW, halfH, halfD);
		vertices[17].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[17].tex = FLOAT2(0.f, 0.f);
		vertices[18].pos = FLOAT3(halfW, halfH, halfD);
		vertices[18].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[18].tex = FLOAT2(1.f, 0.f);
		vertices[19].pos = FLOAT3(halfW, halfH, -halfD);
		vertices[19].normal = FLOAT3(0.f, 1.f, 0.f);
		vertices[19].tex = FLOAT2(1.f, 1.f);

		vertices[20].pos = FLOAT3(-halfW, -halfH, halfD);
		vertices[20].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[20].tex = FLOAT2(0.f, 1.f);
		vertices[21].pos = FLOAT3(-halfW, -halfH, -halfD);
		vertices[21].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[21].tex = FLOAT2(0.f, 0.f);
		vertices[22].pos = FLOAT3(halfW, -halfH, -halfD);
		vertices[22].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[22].tex = FLOAT2(1.f, 0.f);
		vertices[23].pos = FLOAT3(halfW, -halfH, halfD);
		vertices[23].normal = FLOAT3(0.f, -1.f, 0.f);
		vertices[23].tex = FLOAT2(1.f, 1.f);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEBoxVertex) * 24;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		D3D11_SUBRESOURCE_DATA vbData;
		vbData.pSysMem = vertices;
		vbData.SysMemPitch = 0;
		vbData.SysMemSlicePitch = 0;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, &vbData, &m_objectVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBox::CreateBoxIndexBuffer()
	{
		SAFE_RELEASE(m_objectIB);

		UINT indices[36];
		indices[0] = 0;
		indices[1] = 1;
		indices[2] = 2;
		indices[3] = 0;
		indices[4] = 2;
		indices[5] = 3;

		indices[6] = 4;
		indices[7] = 5;
		indices[8] = 6;
		indices[9] = 4;
		indices[10] = 6;
		indices[11] = 7;

		indices[12] = 8;
		indices[13] = 9;
		indices[14] = 10;
		indices[15] = 8;
		indices[16] = 10;
		indices[17] = 11;

		indices[18] = 12;
		indices[19] = 13;
		indices[20] = 14;
		indices[21] = 12;
		indices[22] = 14;
		indices[23] = 15;

		indices[24] = 16;
		indices[25] = 17;
		indices[26] = 18;
		indices[27] = 16;
		indices[28] = 18;
		indices[29] = 19;

		indices[30] = 20;
		indices[31] = 21;
		indices[32] = 22;
		indices[33] = 20;
		indices[34] = 22;
		indices[35] = 23;

		D3D11_BUFFER_DESC ibDesc = { 0 };
		ibDesc.ByteWidth = sizeof(UINT) * 36;
		ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibDesc.CPUAccessFlags = 0;
		ibDesc.MiscFlags = 0;
		ibDesc.StructureByteStride = 0;
		ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
		D3D11_SUBRESOURCE_DATA ibData;
		ibData.pSysMem = indices;
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