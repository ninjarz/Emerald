#include "EEBox.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBox
	//----------------------------------------------------------------------------------------------------
	bool EEBox::s_isBoxInitialized = false;
	ID3D11InputLayout *EEBox::s_boxIL = NULL;
	ID3D11VertexShader *EEBox::s_boxVS = NULL;
	ID3D11PixelShader  *EEBox::s_boxPS = NULL;
	ID3D11Buffer *EEBox::s_boxBuffer = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEBox::InitializeBox()
	{
		if (!s_isBoxInitialized)
		{
			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEBoxShader.hlsl", NULL, NULL, "BoxVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "ShaderCompileError", MB_YESNO);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				else
				{
					MessageBoxW(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				return FALSE;
			}
			result = D3DX11CompileFromFileW(L"EEShader\\EEBoxShader.hlsl", NULL, NULL, "BoxPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
			if (FAILED(result))
			{
				if (errorMessage)
				{
					MessageBoxA(NULL, (char*)errorMessage->GetBufferPointer(), "ShaderCompileError", MB_YESNO);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				else
				{
					MessageBoxW(NULL, L"CompileShader´íÎó!", L"´íÎó", MB_OK);
					SAFE_RELEASE(errorMessage);
					SAFE_RELEASE(vertexShaderBuffer);
					SAFE_RELEASE(pixelShaderBuffer);
				}
				return FALSE;
			}
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_boxVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_boxPS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC boxInputDesc[3] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(boxInputDesc, 3, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_boxIL);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}

			SAFE_RELEASE(errorMessage);
			SAFE_RELEASE(vertexShaderBuffer);
			SAFE_RELEASE(pixelShaderBuffer);

			s_isBoxInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, const FLOAT3& _size)
		:
		EEObject3D(_pos),
		m_size(_size),
		m_boxVB(NULL),
		m_boxIB(NULL)
	{
		InitializeBox();

		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, const FLOAT3& _size, const EETexture& _tex)
		:
		EEObject3D(_pos),
		m_size(_size),
		m_boxVB(NULL),
		m_boxIB(NULL)
	{
		InitializeBox();
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
		m_size(_width, _height, _depth),
		m_boxVB(NULL),
		m_boxIB(NULL)
	{
		InitializeBox();

		CreateBoxVertexBuffer();
		CreateBoxIndexBuffer();
	}

	//----------------------------------------------------------------------------------------------------
	EEBox::EEBox(const FLOAT3& _pos, FLOAT _width, FLOAT _height, FLOAT _depth, const EETexture& _tex)
		:
		EEObject3D(_pos),
		m_size(_width, _height, _depth),
		m_boxVB(NULL),
		m_boxIB(NULL)
	{
		InitializeBox();
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

		if (m_isPositionDirty)
		{
			m_isPositionDirty = false;
		}

		if (m_isScaleDirty || m_isLocalZOrderDirty)
		{
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

			//map the vertices data to the buffer of the box
			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_boxVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, vertices, sizeof(vertices));
			deviceContext->Unmap(m_boxVB, 0);

			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
		}

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
		deviceConstext->IASetInputLayout(s_boxIL);
		UINT stride = sizeof(EEBoxVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_boxVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(m_boxIB, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_boxVS, NULL, 0);
		ID3D11ShaderResourceView *texture = m_tex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_boxPS, NULL, 0);
		deviceConstext->DrawIndexed(36, 0, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBox::CreateBoxVertexBuffer()
	{
		SAFE_RELEASE(m_boxVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEBoxVertex) * 24;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_boxVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEBox::CreateBoxIndexBuffer()
	{
		SAFE_RELEASE(m_boxIB);

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

		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&ibDesc, &ibData, &m_boxIB)))
		{
			MessageBoxW(NULL, L"CreateIndexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}
}