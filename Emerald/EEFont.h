#pragma once
#ifndef _EE_FONT_H_
#define _EE_FONT_H_

#include <string>
#include "EEObject.h"
#include "EETexture.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEFontData
	//----------------------------------------------------------------------------------------------------
	struct EEFontData
	{
		float left, right;
		int size;
	};

	//EEFontVertex
	//----------------------------------------------------------------------------------------------------
	struct EEFontVertex
	{
		FLOAT3 pos;
		FLOAT2 tex;
	};

	//EEFont
	//----------------------------------------------------------------------------------------------------
	class EEFont : public EEObject
	{
	public:
		static bool InitializeFont();

	public:
		static bool s_isFontInitialized;
		static EETexture s_fontTex;
		static EEFontData *s_fontData;
		static FLOAT s_fontWidth;
		static FLOAT s_fontHeight;
		static ID3D11InputLayout *s_fontIL;
		static ID3D11VertexShader *s_fontVS;
		static ID3D11PixelShader  *s_fontPS;

	public:
		EEFont(const FLOAT3& _position, char* _text, const EEColor& _color);
		EEFont(const EEFont& _font);
		~EEFont();

		virtual bool Update();
		virtual bool Render();
		bool AddText(char _text);

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

	//EEFont_APIS
	//----------------------------------------------------------------------------------------------------
	void EEPrint(const FLOAT3& _position, char* _text, const EEColor& _color);
}



#endif