#include "EEFont.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEFont
	//----------------------------------------------------------------------------------------------------
	bool EEFont::s_isFontInitialized = false;
	int EEFont::s_fontWidth = 0;
	int EEFont::s_fontHeight = 16;

	FT_Library EEFont::s_library = NULL;
	FT_Face EEFont::s_face = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEFont::InitializeFont()
	{
		if (!s_isFontInitialized)
		{
			FT_Error error = FT_Init_FreeType(&s_library);
			if (error)
			{
				return false;
			}
			error = FT_New_Face(s_library, "C:/Windows/Fonts/SIMYOU.TTF", 0, &s_face);
			if (error)
			{
				return false;
			}
			error = FT_Set_Pixel_Sizes(s_face, s_fontWidth, s_fontHeight);
			if (error)
			{
				return false;
			}

			s_isFontInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEFont::GetFontBitmap(wchar_t _char)
	{
		InitializeFont();

		FT_Error error = FT_Load_Char(s_face, _char, FT_LOAD_RENDER);
		if (error)
			return EEBitmap();

		int size = s_face->glyph->bitmap.width * s_face->glyph->bitmap.rows;
		unsigned char *src = s_face->glyph->bitmap.buffer;
		std::vector<unsigned char> dst(size * 4);
		switch (s_face->glyph->bitmap.pixel_mode)
		{
		case FT_PIXEL_MODE_NONE:
			break;

		case FT_PIXEL_MODE_MONO:
			for (int i = 0; i < s_face->glyph->bitmap.rows; ++i)
			{
				int srcIndex = i * s_face->glyph->bitmap.pitch;
				for (int j = 0; j < s_face->glyph->bitmap.width; ++j)
				{
					char pixel = (src[srcIndex + (j >> 3)] & (0x80 >> (j & 7))) ? 255 : 0;
					int dstIndex = (i * s_face->glyph->bitmap.width + j) << 2;
					dst[dstIndex + 0] = 255;
					dst[dstIndex + 1] = 255;
					dst[dstIndex + 2] = 255;
					dst[dstIndex + 3] = pixel;
				}
			}
			break;

		case FT_PIXEL_MODE_GRAY:
			// memo:index + 3 should be 255 and other parts of the color should be src[i]
			for (int i = 0; i < size; ++i)
			{
				int dstIndex = i << 2;
				dst[dstIndex + 0] = 255;
				dst[dstIndex + 1] = 255;
				dst[dstIndex + 2] = 255;
				dst[dstIndex + 3] = src[i];
			}
			break;
		}
		//wprintf(L"char:%c left:%d top:%d ax:%d ay:%d\n", _char, s_face->glyph->bitmap_left, s_face->glyph->bitmap_top, s_face->glyph->advance.x >> 6, s_face->glyph->advance.y >> 6);
		EEBitmap result(s_face->glyph->advance.x >> 6, (int)(s_fontHeight * 1.5f));
		result.PutData(s_face->glyph->bitmap_left, s_fontHeight - s_face->glyph->bitmap_top, s_face->glyph->bitmap.width, s_face->glyph->bitmap.rows, EEBitmap(s_face->glyph->bitmap.width, s_face->glyph->bitmap.rows, dst.data()));
		return result;
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEFont::GetFontBitmap(std::wstring _string)
	{
		InitializeFont();

		std::vector<EEBitmap> bitmaps;
		for (wchar_t letter : _string)
		{
			bitmaps.push_back(GetFontBitmap(letter));
		}

		return EEBitmapCombineHorizontal(bitmaps);
	}

	//----------------------------------------------------------------------------------------------------
	EEFont::EEFont(const FLOAT3& _position, const EEColor& _color, wchar_t* _text)
		:
		EEQuad2D(_position),
		m_text(_text),
		m_isTextDirty(true)
	{
		InitializeFont();

		SetColor(_color);
		SetIsUseColor(true);
		SetIsUseTex(true);
	}

	//----------------------------------------------------------------------------------------------------
	EEFont::EEFont(const EEFont& _font)
		:
		EEQuad2D(_font),
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
		if (m_isTextDirty)
		{
			SetTexture(GetFontBitmap(m_text));
			SetWidth((float)GetTexture()->GetWidth());
			SetHeight((float)GetTexture()->GetHeight());

			m_isTextDirty = false;
		}

		if (!EEQuad2D::Update())
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::AddText(wchar_t _text)
	{
		if (_text == 8)
		{
			if (m_text.size())
				m_text.pop_back();
		}
		else if (32 < _text && _text <= 126)
		{
			m_text += _text;
		}
		else
		{
			m_text += _text;
		}
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::AddText(const wchar_t* _text)
	{
		if (*_text == L'\0')
			return false;

		m_text += _text;
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::SetText(wchar_t* _text)
	{
		m_text = _text;
		m_isTextDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	const std::wstring& EEFont::GetText()
	{
		return m_text;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::IsTextDirty()
	{
		return m_isTextDirty;
	}

	//EEFont_APIS
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEGetFontBitmap(wchar_t _char)
	{
		return EEFont::GetFontBitmap(_char);
	}

	EEBitmap EEGetFontBitmap(std::wstring _string)
	{
		return EEFont::GetFontBitmap(_string);
	}

	//----------------------------------------------------------------------------------------------------
	void EEPrint(const FLOAT3& _position, const EEColor& _color, wchar_t* _text)
	{
		EEFont font(_position, _color, _text);
		font.Process();
	}
}


/*
// old version
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

	FT_Library EEFont::s_library = NULL;
	FT_Face EEFont::s_face = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEFont::InitializeFont()
	{
		if (!s_isFontInitialized)
		{
			FT_Error error = FT_Init_FreeType(&s_library);
			if (error)
			{
				return false;
			}
			error = FT_New_Face(s_library, "C:/Windows/Fonts/simsun.ttc", 0, &s_face);
			if (error)
			{
				return false;
			}
			error = FT_Set_Pixel_Sizes(s_face, 0, (int)s_fontHeight);
			if (error)
			{
				return false;
			}

			//----------------------------------------------------------------------------------------------------
			s_fontTex.SetTexture(L"EEFont/EEFont.dds");
			std::ifstream fin(L"EEFont/EEFontData.txt");
			if (fin.is_open())
			{
				s_fontData = new EEFontData[95];

				char temp;
				for (int i = 0; i < 95; i++)
				{
					fin.get(temp);
					while (temp != ' ')
					{
						fin.get(temp);
					}
					fin.get(temp);
					while (temp != ' ')
					{
						fin.get(temp);
					}

					fin >> s_fontData[i].left;
					fin >> s_fontData[i].right;
					fin >> s_fontData[i].size;
				}

				fin.close();
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
	EEBitmap EEFont::GetFontBitmap(wchar_t _char)
	{
		InitializeFont();

		FT_Error error = FT_Load_Char(s_face, _char, FT_LOAD_RENDER);
		if (error)
		{
			return EEBitmap();
		}

		int size = s_face->glyph->bitmap.width * s_face->glyph->bitmap.rows;
		unsigned char *src = s_face->glyph->bitmap.buffer;
		std::vector<unsigned char> dst(size * 4);
		switch (s_face->glyph->bitmap.pixel_mode)
		{
		case FT_PIXEL_MODE_NONE:
			break;

		case FT_PIXEL_MODE_MONO:
			for (int i = 0; i < s_face->glyph->bitmap.rows; ++i)
			{
				int srcIndex = i * s_face->glyph->bitmap.pitch;
				for (int j = 0; j < s_face->glyph->bitmap.width; ++j)
				{
					char pixel = (src[srcIndex + (j >> 3)] & (0x80 >> (j & 7))) ? 255 : 0;
					int dstIndex = (i * s_face->glyph->bitmap.width + j) << 2;
					dst[dstIndex + 0] = 255;
					dst[dstIndex + 1] = 255;
					dst[dstIndex + 2] = 255;
					dst[dstIndex + 3] = pixel;
				}
			}
			break;

		case FT_PIXEL_MODE_GRAY:
			// memo:index + 3 should be 255 and other parts of the color should be src[i]
			for (int i = 0; i < size; ++i)
			{
				int dstIndex = i << 2;
				dst[dstIndex + 0] = 255;
				dst[dstIndex + 1] = 255;
				dst[dstIndex + 2] = 255;
				dst[dstIndex + 3] = src[i];
			}
			break;
		}
		return EEBitmap(dst.data(), s_face->glyph->bitmap.width, s_face->glyph->bitmap.rows);
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEFont::GetFontBitmap(std::wstring _string)
	{
		InitializeFont();

		std::vector<EEBitmap> bitmaps;
		for (wchar_t letter : _string)
		{
			bitmaps.push_back(GetFontBitmap(letter));
		}

		return EEBitmapCombineHorizontal(bitmaps);
	}

	//----------------------------------------------------------------------------------------------------
	EEFont::EEFont(const FLOAT3& _position, const EEColor& _color, char* _text)
		:
		EEObject2D(),
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
		EEObject2D(_font),
		m_text(_font.m_text),
		m_isTextDirty(_font.m_isTextDirty)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEFont::~EEFont()
	{
		SAFE_RELEASE(m_fontVB);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::Update()
	{
		if (!EEObject::Update())
			return false;

		if ((m_isPositionDirty || m_isScaleDirty || m_isLocalZOrderDirty || m_isTextDirty) && m_text.size())
		{
			float fontPosX = 0.f;
			float fontPosY = 0.f;
			float deltaX = s_fontWidth * GetFinalScale().x;
			float deltaY = s_fontHeight * GetFinalScale().y;
			float fontWidth = 0.0f;
			float fontHeight = fontPosY + deltaY;

			std::vector<EEFontVertex> vertices(m_text.size() * 6);
			int length(0), index(0), letter(0);
			for (unsigned int i = 0; i < m_text.size(); ++i)
			{
				letter = ((int)m_text[i]);
				if (letter == '\r')
				{
					fontPosX = 0.f;
					fontPosY += deltaY;
					fontHeight = fontPosY + deltaY;
				}
				else if (letter == 32)
				{
					fontPosX = fontPosX + 3 * deltaX;
				}
				else if (32 < letter && letter <= 126)
				{
					++length;
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
			if (vertices.size())
			{
				ID3D11DeviceContext *deviceContext = EECore::s_EECore->GetDeviceContext();
				D3D11_MAPPED_SUBRESOURCE mappedResource;
				ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
				deviceContext->Map(m_fontVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
				memcpy(mappedResource.pData, &vertices[0], sizeof(EEFontVertex)* length * 6);
				deviceContext->Unmap(m_fontVB, 0);
			}

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
		if (!EEObject::Render())
			return false;

		if (m_text.size())
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
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFont::AddText(char _text)
	{
		if (_text == 8)
		{
			if (m_text.size())
				m_text.pop_back();
		}
		else if (32 < _text && _text <= 126)
		{
			m_text += _text;
		}
		else
		{
			m_text += _text;
		}
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
	EEBitmap EEGetFontBitmap(wchar_t _char)
	{
		return EEFont::GetFontBitmap(_char);
	}

	EEBitmap EEGetFontBitmap(std::wstring _string)
	{
		return EEFont::GetFontBitmap(_string);
	}

	//----------------------------------------------------------------------------------------------------
	void EEPrint(const FLOAT3& _position, const EEColor& _color, char* _text)
	{
		EEFont font(_position, _color, _text);
		font.Process();
	}
}
*/