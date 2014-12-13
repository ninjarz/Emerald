#include "EEFont.h"
#include "EECore.h"
#include <fstream>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEFont
	//----------------------------------------------------------------------------------------------------
	bool EEFont::s_isFontInitialized = false;
	EETexture EEFont::s_fontTex;
	EEFontData *EEFont::s_fontData = NULL;
	FLOAT EEFont::s_fontWidth = 1.0f;
	FLOAT EEFont::s_fontHeight = 16.0f;
	ID3D11InputLayout *EEFont::s_fontIL = NULL;
	ID3D11VertexShader *EEFont::s_fontVS = NULL;
	ID3D11PixelShader  *EEFont::s_fontPS = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEFont::InitializeFont()
	{
		if (!s_isFontInitialized)
		{
			s_fontTex.LoadTextureFromFile(L"EEFont/EEFont.dds");
			std::ifstream fIn("EEFont/EEFontData.txt");
			if (fIn.is_open())
			{
				s_fontData = new EEFontData[95];

				char temp;
				for (int i = 0; i < 95; i++)
				{
					fIn.get(temp);
					while (temp != ' ')
					{
						fIn.get(temp);
					}
					fIn.get(temp);
					while (temp != ' ')
					{
						fIn.get(temp);
					}

					fIn >> s_fontData[i].left;
					fIn >> s_fontData[i].right;
					fIn >> s_fontData[i].size;
				}

				fIn.close();
			}
			else
				return false;

			HRESULT result;
			ID3D10Blob *errorMessage = NULL;
			ID3D10Blob *vertexShaderBuffer = NULL;
			ID3D10Blob *pixelShaderBuffer = NULL;

			result = D3DX11CompileFromFileW(L"EEShader\\EEFontShader.hlsl", NULL, NULL, "FontVS", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
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
			result = D3DX11CompileFromFileW(L"EEShader\\EEFontShader.hlsl", NULL, NULL, "FontPS", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
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
			result = EECore::s_EECore->GetDevice()->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &s_fontVS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			result = EECore::s_EECore->GetDevice()->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &s_fontPS);
			if (FAILED(result))
			{
				SAFE_RELEASE(errorMessage);
				SAFE_RELEASE(vertexShaderBuffer);
				SAFE_RELEASE(pixelShaderBuffer);
				return false;
			}
			D3D11_INPUT_ELEMENT_DESC quadInputDesc[2] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			result = EECore::s_EECore->GetDevice()->CreateInputLayout(quadInputDesc, 2, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &s_fontIL);
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

			s_isFontInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEFont::EEFont(const FLOAT3& _position, char* _text, const EEColor& _color)
		:
		EEObject(),
		m_text(_text),
		m_isTextDirty(true)
	{
		InitializeFont();

		SetPosition(_position);
		SetColor(_color);
	}

	//----------------------------------------------------------------------------------------------------
	EEFont::EEFont(const EEFont& _font)
		:
		EEObject(_font),
		m_text(_font.m_text),
		m_isTextDirty(_font.m_isTextDirty)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEFont::~EEFont()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::Update()
	{
		if ((m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty || m_isTextDirty) && m_text.size())
		{
			float fontPosX = m_position.x;
			float fontPosY = m_position.y;
			float deltaX = s_fontWidth * GetFinalScale().x;
			float deltaY = s_fontHeight * GetFinalScale().y;
			float fontWidth = 0.0f;
			float fontHeight = fontPosY + deltaY;

			int length = m_text.size();
			std::vector<EEFontVertex> vertices(length * 6);
			int index(0), letter(0);
			for (int i = 0; i < length; ++i)
			{
				letter = ((int)m_text[i]);
				if (letter == 32)
				{
					fontPosX = fontPosX + 3 * deltaX;
				}
				else if (32 < letter && letter <= 126)
				{
					letter -= 32;
					fontWidth = fontPosX + s_fontData[letter].size * deltaX;

					vertices[index].pos = FLOAT3(fontPosX, fontPosY, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].left, 0.0f);
					index++;
					vertices[index].pos = FLOAT3(fontWidth, fontHeight, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].right, 1.0f);
					index++;
					vertices[index].pos = FLOAT3(fontPosX, fontHeight, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].left, 1.0f);
					index++;

					vertices[index].pos = FLOAT3(fontPosX, fontPosY, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].left, 0.0f);
					index++;
					vertices[index].pos = FLOAT3(fontWidth, fontPosY, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].right, 0.0f);
					index++;
					vertices[index].pos = FLOAT3(fontWidth, fontHeight, 0.0f);
					vertices[index].tex = FLOAT2(s_fontData[letter].right, 1.0f);
					index++;

					fontPosX += s_fontData[letter].size * deltaX + deltaX;
				}
			}
			if (m_isTextDirty)
				CreateFontVertexBuffer(length * 6);
			ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
			D3D11_MAPPED_SUBRESOURCE mappedResource;
			ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
			deviceContext->Map(m_fontVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			memcpy(mappedResource.pData, &vertices[0], sizeof(EEFontVertex) * length * 6);
			deviceContext->Unmap(m_fontVB, 0);

			m_isPositionDirty = false;
			m_isScaleDirty = false;
			m_isLocalZOrderDirty = false;
			m_isTextDirty = false;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::Render()
	{
		MapObjectBuffer();

		ID3D11DeviceContext *deviceConstext = EECore::s_EECore->GetDeviceContext();
		deviceConstext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceConstext->IASetInputLayout(s_fontIL);
		UINT stride = sizeof(EEFontVertex);
		UINT offset = 0;
		deviceConstext->IASetVertexBuffers(0, 1, &m_fontVB, &stride, &offset);
		deviceConstext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		deviceConstext->VSSetShader(s_fontVS, NULL, 0);
		ID3D11ShaderResourceView *texture = s_fontTex.GetTexture();
		deviceConstext->PSSetShaderResources(0, 1, &texture);
		deviceConstext->PSSetShader(s_fontPS, NULL, 0);
		deviceConstext->Draw(m_text.size() * 6, 0);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::AddText(char _text)
	{
		m_text += _text;
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::AddText(const char* _text)
	{
		m_text += _text;
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::SetText(char* _text)
	{
		m_text = _text;
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	const std::string& EEFont::GetText()
	{
		return m_text;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::IsTextDirty()
	{
		return m_isTextDirty;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::CreateFontVertexBuffer(int _verticesNum)
	{
		if (!_verticesNum)
			return false;

		SAFE_RELEASE(m_fontVB);

		D3D11_BUFFER_DESC vbDesc = { 0 };
		vbDesc.ByteWidth = sizeof(EEFontVertex) * _verticesNum;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vbDesc.MiscFlags = 0;
		vbDesc.StructureByteStride = 0;
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		if (FAILED(EECore::s_EECore->GetDevice()->CreateBuffer(&vbDesc, NULL, &m_fontVB)))
		{
			MessageBoxW(NULL, L"CreateVertexBuffer failed!", L"Error", MB_OK);
			return false;
		}

		return true;
	}

	//EEFont_APIS
	//----------------------------------------------------------------------------------------------------
	void EEPrint(const FLOAT3& _position, char* _text, const EEColor& _color)
	{
		EEFont font(_position, _text, _color);
		font.Process();
	}
}