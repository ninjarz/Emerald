#pragma once
#ifndef _EE_LISTVIEW_H_
#define _EE_LISTVIEW_H_

#include "EESCrollView.h"
#include "EEList.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEListView
	//----------------------------------------------------------------------------------------------------
	class EEListView : public EEScrollView
	{
	public:
		EEListView();
		EEListView(const Rect_Float& _area);
		EEListView(const EEListView& _listView);
		virtual ~EEListView();

		virtual bool Update();
		virtual bool Render();
		virtual bool Process();
		bool AddObject(EEObject* _object);
		bool AddObjectAt(EEObject* _object, int _index);
		bool RemoveObject(EEObject* _object);
		bool RemoveObjectAt(int _index);

	protected:
		bool SetContent(EEObject* _content);

	protected:
		EEList m_list;
	};
}


#endif