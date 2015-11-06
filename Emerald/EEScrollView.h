#pragma once
#ifndef _EE_SCROLLVIEW_H_
#define _EE_SCROLLVIEW_H_

#include "EEQuad2D.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEScrollView
	//----------------------------------------------------------------------------------------------------
	class EEScrollView : public EEQuad2D
	{
	public:
		EEScrollView(const Rect_Float& _area);
		EEScrollView(const EEScrollView& _scrollView);
		virtual ~EEScrollView();

		virtual bool Update();
		virtual bool Render();

		

	protected:

	};
}


#endif