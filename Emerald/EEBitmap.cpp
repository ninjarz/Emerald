#include "EEBitmap.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBitmap
	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap()
		:
		m_data(),
		m_width(0),
		m_height(0)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(const unsigned char* _buffer, unsigned int _width, unsigned int _height)
		:
		m_data(_buffer, _buffer + _width * _height * 4),
		m_width(_width),
		m_height(_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::EEBitmap(const EEBitmap& _bitmap)
		:
		m_data(_bitmap.m_data),
		m_width(_bitmap.m_width),
		m_height(_bitmap.m_height)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap::~EEBitmap()
	{

	}

	//----------------------------------------------------------------------------------------------------
	unsigned char* EEBitmap::GetData()
	{
		return m_data.data();
	}

	//----------------------------------------------------------------------------------------------------
	int EEBitmap::GetWidth() const
	{
		return m_width;
	}

	//----------------------------------------------------------------------------------------------------
	int EEBitmap::GetHeight() const
	{
		return m_height;
	}

	//EEBitmap_APIs
	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineHorizontal(EEBitmap& _left, EEBitmap& _right)
	{
		unsigned int* srcL = (unsigned int*)_left.GetData();
		unsigned int* srcR = (unsigned int*)_right.GetData();
		int widthL = _left.GetWidth();
		int widthR = _right.GetWidth();
		int heightL = _left.GetHeight();
		int heightR = _right.GetHeight();

		int width = widthL + widthR;
		if (heightL >= heightR)
		{
			int height = heightL;
			std::vector<unsigned int> buffer(width * height, (unsigned int)0);
			for (int i = 0; i < heightR; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex], &srcL[i * widthL], widthL << 2);
				memcpy(&buffer[dstIndex + widthL], &srcR[i * widthR], widthR << 2);
			}
			for (int i = heightR + 1; i < heightL; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex], &srcL[i * widthL], widthL << 2);
			}
			return EEBitmap((unsigned char*)buffer.data(), width, height);
		}
		else
		{
			int height = heightR;
			std::vector<unsigned int> buffer(width * height, (unsigned int)0);
			for (int i = 0; i < heightL; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex], &srcL[i * widthL], widthL << 2);
				memcpy(&buffer[dstIndex + widthL], &srcR[i * widthR], widthR << 2);
			}
			for (int i = heightR + 1; i < heightR; ++i)
			{
				int dstIndex = i * width;
				memcpy(&buffer[dstIndex + widthL], &srcR[i * widthR], widthR << 2);
			}
			return EEBitmap((unsigned char*)buffer.data(), width, height);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEBitmap EEBitmapCombineVertical(EEBitmap& _top, EEBitmap& _bottom)
	{
		return _top;
	}

}