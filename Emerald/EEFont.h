#pragma once
#ifndef _EE_FONT_H_
#define _EE_FONT_H_

// freetype-2.5.3
#ifdef _DEBUG
#pragma comment(lib, "freetype253MT_D.lib")
#else
#pragma comment(lib, "freetype253MT.lib")
#endif
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H 
#include <string>
#include "EEQuad2D.h"
#include "EETexture.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEFont
	//----------------------------------------------------------------------------------------------------
	class EEFont : public EEQuad2D
	{
	public:
		static bool InitializeFont();
		static EEBitmap GetFontBitmap(wchar_t _char);
		static EEBitmap GetFontBitmap(std::wstring _string);

	public:
		static bool s_isFontInitialized;
		static int s_fontWidth;
		static int s_fontHeight;

		static FT_Library s_library;
		static FT_Face s_face;

	public:
		EEFont(const FLOAT3& _position, const EEColor& _color, wchar_t* _text);
		EEFont(const EEFont& _font);
		virtual ~EEFont();

		virtual bool Update();
		virtual bool Render();
		bool AddText(wchar_t _text);
		bool AddText(const wchar_t* _text);

		bool SetText(wchar_t* _text);

		const std::wstring& GetText();
		bool IsTextDirty();

	protected:
		// memo:font's width and height
		std::wstring m_text;
		bool m_isTextDirty;
	};

	// EEFont_APIS
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEGetFontBitmap(wchar_t _char);
	EEBitmap EEGetFontBitmap(std::wstring _string);
	void EEPrint(const FLOAT3& _position, const EEColor& _color, wchar_t* _text);
}

/*
// old version
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEFontData
	//----------------------------------------------------------------------------------------------------
	struct EEFontData
	{
		float left, right;
		int size;
	};

	// EEFontVertex
	//----------------------------------------------------------------------------------------------------
	struct EEFontVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	// EEFont
	//----------------------------------------------------------------------------------------------------
	class EEFont : public EEObject2D
	{
	public:
		static bool InitializeFont();
		static EEBitmap GetFontBitmap(wchar_t _char);
		static EEBitmap GetFontBitmap(std::wstring _string);

	public:
		static bool s_isFontInitialized;
		static EETexture s_fontTex;
		static EEFontData *s_fontData;
		static FLOAT s_fontWidth;
		static FLOAT s_fontHeight;
		static ID3D11InputLayout *s_fontIL;
		static ID3D11VertexShader *s_fontVS;
		static ID3D11PixelShader  *s_fontPS;

		static FT_Library s_library;
		static FT_Face s_face;

	public:
		EEFont(const FLOAT3& _position, const EEColor& _color, char* _text);
		EEFont(const EEFont& _font);
		virtual ~EEFont();

		virtual bool Update();
		virtual bool Render();
		bool AddText(char _text);
		bool AddText(const char* _text);

		bool SetText(char* _text);

		const std::string& GetText();
		bool IsTextDirty();

	protected:
		bool CreateFontVertexBuffer(int _verticesNum);

	protected:
		std::string m_text;
		bool m_isTextDirty;
		ID3D11Buffer *m_fontVB;
	};

	// EEFont_APIS
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEGetFontBitmap(wchar_t _char);
	EEBitmap EEGetFontBitmap(std::wstring _string);
	void EEPrint(const FLOAT3& _position, const EEColor& _color, char* _text);
}
*/



#endif