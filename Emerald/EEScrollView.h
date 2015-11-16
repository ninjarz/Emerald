#pragma once
#ifndef _EE_SCROLLVIEW_H_
#define _EE_SCROLLVIEW_H_

#include "EEQuad2D.h"


// todo: EEScrollView(EEObject), m_contentFrame(EEQuad2D)
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
		bool VerticalPull(float _scrollPos);
		bool Repull();

		bool SetContent(EEObject* _content);


	protected:
		EEObject m_contentFrame;
		EEObject* m_content;
		float m_offsetHeight;
		float m_verticalScrollHeight;
		float m_verticalScrollCapacity;
		float m_verticalScrollPos;
		float m_verticalPos;
	};
}


#endif