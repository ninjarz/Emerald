#pragma once
#ifndef _EE_SCROLLVIEW_H_
#define _EE_SCROLLVIEW_H_

#include "EEQuad2D.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEScrollView
	//----------------------------------------------------------------------------------------------------
	/*
	 +--------------------+ content
	 |                    |
	 |                    |
	 |                    |
	++--------------------+-+ view
	||                    | |
	||                    | |
	||                    | | (scroll bar)
	||                    | |
	||                    | |
	++--------------------+-+
	 |    (scroll bar)    |
	 |                    |
	 +--------------------+
	*/
	class EEScrollView : public EEQuad2D
	{
	public:
		EEScrollView(const Rect_Float& _area);
		EEScrollView(const EEScrollView& _scrollView);
		virtual ~EEScrollView();

		virtual bool Update();
		virtual bool Render();
		bool Pull(int _scrollPos);
		bool Repull();

		bool SetContent(EEObject* _content);

	protected:
		EEObject m_content;
		int m_offsetHeight;
		int m_verticalScrollHeight;
		int m_verticalScrollCapacity;
		int m_verticalScrollPos;
		int m_verticalPos;
	};
}


#endif