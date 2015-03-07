#pragma once
#ifndef _EE_BITMAP_H_
#define _EE_BITMAP_H_

#include <d3d11.h>
#include <d3dx11async.h>
#include <vector>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBitmap
	//----------------------------------------------------------------------------------------------------
	class EEBitmap
	{
	public:
		EEBitmap();
		EEBitmap(wchar_t* _file);
		EEBitmap(unsigned int _width, unsigned int _height);
		EEBitmap(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		EEBitmap(const EEBitmap& _bitmap);
		virtual ~EEBitmap();

		unsigned char* GetData();
		int GetDataSize();
		int GetWidth() const;
		int GetHeight() const;
		bool IsEmpty() const;
		EEBitmap GetSubmap(int _x, int _y, int _width = -1, int _height = -1) const;

		bool SetData(const unsigned char* _buffer, unsigned int _width, unsigned int _height);

	protected:
		std::vector<unsigned char> m_data;
		int m_width;
		int m_height;
	};

	//EEBitmap_APIs
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap& _top, EEBitmap& _bottom);
	EEBitmap EEBitmapCombineHorizontal(EEBitmap* _bitmaps);
	EEBitmap EEBitmapCombineVertical(EEBitmap& _top, EEBitmap& _bottom);
	void EEBitmapDivideHorizontal(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps);
	void EEBitmapDivideVertical(EEBitmap& _bitmap, unsigned int _amount, std::vector<EEBitmap>& _bitmaps);
}

#endif