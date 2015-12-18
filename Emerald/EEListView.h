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
	class EEListView : protected EEScrollView
	{
	public:
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
		EEList m_list;
	};
}


#endif