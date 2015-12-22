#pragma once
#ifndef _EE_LIST_H_
#define _EE_LIST_H_

#include <vector>
#include <utility>
#include "EEGeometry.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEListItem
	//----------------------------------------------------------------------------------------------------
	struct EEListItem
	{
		EEObject* anchor;
		EEObject* content;

		inline EEListItem(EEObject* _anchor,EEObject* _content)
			:
			anchor(_anchor),
			content(_content)
		{}
	};

	// EEList
	//----------------------------------------------------------------------------------------------------
	class EEList : public EEObject
	{
	public:
		EEList();
		EEList(const EEList &_list);
		virtual ~EEList();

		virtual bool Update();
		virtual bool Render();
		bool AddObject(EEObject* _object);
		bool AddObjectAt(EEObject* _object, unsigned int _index);
		bool RemoveObject(EEObject* _object);
		bool RemoveObjectAt(unsigned int _index);

	protected:
		std::vector<EEListItem> m_objects;
	};
}


#endif