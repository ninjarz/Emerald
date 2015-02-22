#pragma once
#ifndef _EE_BITMAP_H_
#define _EE_BITMAP_H_

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
		EEBitmap(const unsigned char* _buffer, unsigned int _width, unsigned int _height);
		EEBitmap(const EEBitmap& _bitmap);
		~EEBitmap();

		unsigned char* GetData();
		int GetWidth() const;
		int GetHeight() const;
		EEBitmap GetSubmap(int _x, int _y, int _width = -1, int _height = -1) const;

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
}

#endif