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
		//----------------------------------------------------------------------------------------------------
		EESmartPtr(_T *_value = nullptr)
			:
			m_counter(new int(1)),
			m_value(_value)
		{
			// std::cout << "init:" << *m_counter << std::endl;
		}

		//----------------------------------------------------------------------------------------------------
		EESmartPtr(const EESmartPtr &_smartPtr)
			:
			m_counter(_smartPtr.m_counter),
			m_value(_smartPtr.m_value)
		{
			// std::cout << "copy:" << *m_counter << std::endl;
			++*m_counter;
		}

		//----------------------------------------------------------------------------------------------------
		virtual ~EESmartPtr()
		{
			DecCounter();
		}

		//----------------------------------------------------------------------------------------------------
		EESmartPtr& operator= (const EESmartPtr &_smartPtr)
		{
			DecCounter();
			++*_smartPtr.m_counter;

			m_counter = _smartPtr.m_counter;
			m_value = _smartPtr.m_value;

			return *this;
		}

		//----------------------------------------------------------------------------------------------------
		bool operator== (const EESmartPtr &_smartPtr) const
		{
			return m_counter == _smartPtr.m_counter && m_value == _smartPtr.m_value;
		}

		//----------------------------------------------------------------------------------------------------
		bool operator!= (const EESmartPtr &_smartPtr) const
		{
			return !(*this == _smartPtr);
		}

		//----------------------------------------------------------------------------------------------------
		bool operator< (const EESmartPtr &_smartPtr) const
		{
			return m_value < _smartPtr.m_value;
		}

		//----------------------------------------------------------------------------------------------------
		_T* operator-> ()
		{
			return m_value;
		}

		//----------------------------------------------------------------------------------------------------
		_T& operator* ()
		{
			return *m_value;
		}

		//----------------------------------------------------------------------------------------------------
		operator bool ()
		{
			return m_value ? true : false;
		}

		// Dangerous
		//----------------------------------------------------------------------------------------------------
		/*
		operator _T* ()
		{ 
			return m_value;
		}
		*/


	protected:
		void SetValue(_T *_value)
		{
			DecCounter();

			m_counter = new int(1);
			m_value = _value;
		}

		void DecCounter()
		{
			if (--*m_counter == 0)
			{
				// std::cout << "delete:" << *m_counter << std::endl;
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