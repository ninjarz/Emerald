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
content origin
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
		EEScrollView();
		EEScrollView(const Rect_Float& _area);
		EEScrollView(const EEScrollView& _scrollView);
		virtual ~EEScrollView();

		virtual bool Update();
		virtual bool Render();

	public:
		bool VerticalPull(float _scrollPos);
		bool Repull();

		bool SetContent(EEObject* _content);

	protected:
		EEObject m_contentOrigin;
		EEObject* m_content;
		float m_offsetHeight;
		float m_verticalScrollHeight;
		float m_verticalScrollCapacity;
		float m_verticalScrollPos;
		float m_verticalPos;
		float m_verticalPosDelta;
	};
}


#endif