#pragma once
#ifndef _EE_THREAD_H_
#define _EE_THREAD_H_

#include <list>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

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
		~EEThreadSystem();

		bool Initialize();
		void Shutdown();

		bool AddThread(boost::thread* _thread);
		bool RemoveThread(boost::thread* _thread);
		void JoinAll();

	private:
		std::list<boost::thread*> m_threads;
	};

	//EEThread_APIs
	//----------------------------------------------------------------------------------------------------
	void EEThreadSleep(int _millisecs);
	bool EEAddThread(boost::thread* _thread);
	bool EERemoveThread(boost::thread* _thread);
	void EEJoinAll();
}

#endif