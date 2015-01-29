#include "EEThread.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEThread
	//----------------------------------------------------------------------------------------------------
	EEThreadSystem::EEThreadSystem()
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEThreadSystem::EEThreadSystem(const EEThreadSystem& _system)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEThreadSystem::~EEThreadSystem()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEThreadSystem::Initialize()
	{

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEThreadSystem::Shutdown()
	{
		for (boost::thread* thread : m_threads)
		{
			thread->interrupt();
			thread->join();
			delete thread;
		}
		m_threads.clear();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEThreadSystem::AddThread(boost::thread* _thread)
	{
		if (_thread)
		{
			m_threads.push_back(_thread);
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEThreadSystem::RemoveThread(boost::thread* _thread)
	{
		auto it = std::find(m_threads.begin(), m_threads.end(), _thread);
		if (it != m_threads.end())
		{
			(*it)->interrupt();
			delete *it;
			m_threads.erase(it);
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	void EEThreadSystem::JoinAll()
	{
		for (auto it = m_threads.begin(), end = m_threads.end(); it != end; ++it)
		{
			(*it)->join();
		}
	}

	//EEThread_APIs
	//----------------------------------------------------------------------------------------------------
	void EEThreadSleep(int _millisecs)
	{
		boost::this_thread::sleep(boost::posix_time::milliseconds(_millisecs));
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAddThread(boost::thread* _thread) { return EECore::s_EECore->GetEEThreadSystem()->AddThread(_thread); }

	//----------------------------------------------------------------------------------------------------
	bool EERemoveThread(boost::thread* _thread) { return EECore::s_EECore->GetEEThreadSystem()->RemoveThread(_thread); }

	//----------------------------------------------------------------------------------------------------
	void EEJoinAll() { EECore::s_EECore->GetEEThreadSystem()->JoinAll(); }
}