#pragma once
#ifndef _EE_TIMER_H_
#define _EE_TIMER_H_

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETimer
	//----------------------------------------------------------------------------------------------------
	class EETimer
	{
	public:
		inline EETimer()
		{
		}

		EETimer(const EETimer&)
		{
		}

		virtual ~EETimer()
		{
			Shutdown();
		}

		bool Initialize();
		void Shutdown();

		void Start();				//暂停后恢复计时
		void Stop();				//暂停
		void Reset();				//重置，开始计时，定时器开始工作时调用
		void Tick();				//计时一次，每一帧调用一次
		double GetTotalTime() const;	//返回定时器从开始计时到现在的总时间（不包含暂停时间），单位：秒
		double GetDeltaTime() const;	//返回本次计时与上次计时之间的时间差，单位：秒
		int GetFPS() const;
		void ShowFPSInTitle();
		void ShowFPSInTitle(LPCWSTR _title);

	private:
		void CountPFS();

	private:
		double	m_secsPerCount;		//系统有关，两个系统计数器之间的时间差值，单位：秒
		double	m_deltaTime;		//距离上次计时之间的时间差，单位：秒
		__int64 m_baseTime;			//定时器开始工作的起始时间点
		__int64 m_stopTime;			//如果当前为暂停状态，即指暂时的起始时刻
		__int64 m_pausedTime;		//暂停的总时间累计
		__int64	m_currTime;			//本次计时的时间点
		__int64	m_prevTime;			//上次计时的时间点
		bool m_isStopped;			//是否暂停
		int m_FPS;
	};

	//EETimer_APIs
	//----------------------------------------------------------------------------------------------------
	void EETimerStart();
	void EETimerStop();
	void EETimerReset();
	void EETimerTick();
	double EEGetTotalTime();
	double EEGetDeltaTime();
	int EEGetFPS();
	void EEShowFPSInTitle();
	void EEShowFPSInTitle(LPCWSTR _title);
}


#endif