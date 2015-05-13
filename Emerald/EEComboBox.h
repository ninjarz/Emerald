#pragma once
#ifndef _EE_COMBOBOX_H_
#define _EE_COMBOBOX_H_

#include "EEGeometry.h"
#include "EELineBrowser.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEButtonType
	//----------------------------------------------------------------------------------------------------
	class EEComboBox : public EELineBrowser
	{
	public:
		EEComboBox(const Rect_Float& _rect, const EEColor& _color, const EEColor& _fontColor);
		virtual ~EEComboBox();

		virtual bool Update();
		virtual bool Render();
		bool AddItem(const std::wstring& _str, std::function<void(void)> _funcPtr = []{});

		virtual void OnMouseClicked(const Point& _pos);

	protected:
		bool m_isSelecting;
		std::vector<EELineBrowser*> m_options;
		int m_selected;
		bool m_isSelectedDirty;
	};
}

#endif