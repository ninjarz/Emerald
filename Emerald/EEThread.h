#pragma once
#ifndef _EE_THREAD_H_
#define _EE_THREAD_H_

#include <list>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

// not useful now
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEThreadSystem
	//----------------------------------------------------------------------------------------------------
	class EEThreadSystem
	{
	public:
		EEThreadSystem();
		EEThreadSystem(const EEThreadSystem& _system);
		inline virtual ~EEThreadSystem() { Shutdown(); }

		bool Initialize();
		void Shutdown();

		bool AddThread(boost::thread* _thread);
		bool RemoveThread(boost::thread* _thread);
		bool RemoveThread();
		void JoinAll();

	private:
		std::list<boost::thread*> m_threads;
	};

	//EEThread_APIs
	//----------------------------------------------------------------------------------------------------
	void EEThreadSleep(int _millisecs);
	bool EEAddThread(boost::thread* _thread);
	bool EERemoveThread(boost::thread* _thread);
	bool EERemoveThread();
	void EEJoinAll();
}

#endif