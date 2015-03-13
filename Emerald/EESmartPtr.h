#pragma once
#ifndef _EE_SMARTPTR_H_
#define _EE_SMARTPTR_H_


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESmartPtr
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EESmartPtr
	{
	public:
		EESmartPtr(_T *_value = nullptr)
			:
			m_counter(new int(1)),
			m_value(_value)
		{

		}
		EESmartPtr(const EESmartPtr &_smartPtr)
			:
			m_counter(_smartPtr.m_counter),
			m_value(_smartPtr.m_value)
		{
			//std::cout << *m_counter << std::endl;
			++*m_counter;
		}
		virtual ~EESmartPtr()
		{
			DecCounter();
		}

		EESmartPtr& operator= (const EESmartPtr &_smartPtr)
		{
			DecCounter();
			++*_smartPtr.m_counter;

			m_counter = _smartPtr.m_counter;
			m_value = _smartPtr.m_value;

			return *this;
		}

	protected:
		void SetValue(_T *_value)
		{
			DecCounter();

			m_counter = new int(1);
			m_value = _value;
		}

		void DecCounter()
		{
			//std::cout << *m_counter << std::endl;
			if (--*m_counter == 0)
			{
 				delete m_counter;
				m_counter = nullptr;
				if (m_value)
				{
					delete m_value;
					//m_value->Release();
					m_value = nullptr;
				}
			}
		}

	protected:
		int *m_counter;
		_T *m_value;
	};
}

#endif