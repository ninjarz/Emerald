#include "EETimer.h"
#include "EECore.h"

namespace Emerald
{
	//EETimer
	//----------------------------------------------------------------------------------------------------
	bool EETimer::Initialize()
	{
		__int64 frequency;
		//收集系统信息，初始化计数器之间的时间差
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
		m_secsPerCount = 1.f / frequency;
		m_deltaTime = 0.f;
		m_baseTime = 0;
		m_stopTime = 0;
		m_pausedTime = 0;
		m_currTime = 0;
		m_prevTime = 0;
		m_isStopped = false;

		Tick();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::Shutdown()
	{
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::Start()
	{
		//暂停恢复
		//如果本来非暂停状态，则不操作
		//如果为暂停状态
		if (m_isStopped)
		{
			//先获取开始时刻
			__int64 startTime;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

			//把本次暂停时间累计到总暂停时间中
			m_pausedTime += (startTime - m_stopTime);
			m_prevTime = startTime;
			m_stopTime = 0;
			m_isStopped = false;
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::Stop()
	{
		if (!m_isStopped)
		{
			__int64 curr;
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
			m_stopTime = curr;
			m_isStopped = true;
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::Reset()
	{
		//重置定时器，进入工作状态
		__int64 curr;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
		//确定起始时间
		m_baseTime = curr;
		m_currTime = curr;
		m_prevTime = curr;
		m_deltaTime = 0.f;

		m_stopTime = 0;
		//暂停累计清零
		m_pausedTime = 0;
		m_isStopped = false;
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::Tick()
	{
		//if (EECore::s_EECore->GetControlMutex())
		//	boost::unique_lock<boost::shared_mutex> lock(*EECore::s_EECore->GetControlMutex());
		//每帧调用的计时函数
		//用于更新定时器状态

		//如果为暂停状态，则不操作
		if (m_isStopped)
		{
			m_deltaTime = 0.f;
			return;
		}

		//先获取当前时间
		__int64 curr;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&curr));
		m_currTime = curr;
		//计算距离上一次计时的时间差
		m_deltaTime = (m_currTime - m_prevTime)*m_secsPerCount;
		//确保为正数(其实一般情况下不可能为负)
		m_deltaTime = m_deltaTime<0 ? 0.f : m_deltaTime;

		//把上次时间更新为本次时间，供下次Tick用
		m_prevTime = m_currTime;

		CountPFS();
	}

	//----------------------------------------------------------------------------------------------------
	double EETimer::GetTotalTime() const
	{
		//如果是暂停状态，如下图所示：
		//	开始时刻T0			暂停起始时刻S	当前时刻T
		//		|---------------------|------------|
		//总的时间为:S - T0 - 暂停的累计时间
		//注意把计数转换为秒
		if (m_isStopped)
		{
			return (m_stopTime - m_baseTime - m_pausedTime)*m_secsPerCount;
		}
		//如果是正常状态
		//	开始时刻T0							当前时刻T
		//		|----------------------------------|
		//总的时间为：T - T0 - 暂停的累计时间
		//注意把计数转换为秒
		else
		{
			return (m_currTime - m_baseTime - m_pausedTime)*m_secsPerCount;
		}
	}

	//----------------------------------------------------------------------------------------------------
	double EETimer::GetDeltaTime() const
	{
		//返回离上次计时中间的时间差
		return m_deltaTime;
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::ShowFPSInTitle()
	{
		static int tmp = 0;
		if (tmp != m_FPS)
		{
			tmp = m_FPS;
			wchar_t text[50];
			swprintf_s(text, L"%s%d", L"FPS:", m_FPS);
			SetWindowTextW(EECore::s_EECore->GetHWnd(), text);
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::ShowFPSInTitle(LPCWSTR _title)
	{
		static int tmp = 0;
		if (tmp != m_FPS)
		{
			tmp = m_FPS;
			wchar_t text[50];
			swprintf_s(text, L"%s\t%s%d ", _title, L"FPS:", m_FPS);
			SetWindowTextW(EECore::s_EECore->GetHWnd(), text);
		}
	}

	//----------------------------------------------------------------------------------------------------
	void EETimer::CountPFS()
	{
		static float begin = (float)GetTotalTime();
		static int frameCounter = 0;
		if ((float)GetTotalTime() - begin >= 1.f)
		{
			m_FPS = frameCounter;
			begin = (float)GetTotalTime();
			frameCounter = 0;
		}
		++frameCounter;
	}

	//EETimer_APIs
	//----------------------------------------------------------------------------------------------------
	void EETimerStart() { EECore::s_EECore->GetEETimer()->Start(); };

	//----------------------------------------------------------------------------------------------------
	void EETimerStop() { EECore::s_EECore->GetEETimer()->Stop(); };

	//----------------------------------------------------------------------------------------------------
	void EETimerReset() { EECore::s_EECore->GetEETimer()->Reset(); };

	//----------------------------------------------------------------------------------------------------
	void EETimerTick() { EECore::s_EECore->GetEETimer()->Tick(); };

	//----------------------------------------------------------------------------------------------------
	double EEGetTotalTime() { return EECore::s_EECore->GetEETimer()->GetTotalTime(); };

	//----------------------------------------------------------------------------------------------------
	double EEGetDeltaTime() { return EECore::s_EECore->GetEETimer()->GetDeltaTime(); };

	//----------------------------------------------------------------------------------------------------
	void EEShowFPSInTitle() { EECore::s_EECore->GetEETimer()->ShowFPSInTitle(); };

	//----------------------------------------------------------------------------------------------------
	void EEShowFPSInTitle(LPCWSTR _title) { EECore::s_EECore->GetEETimer()->ShowFPSInTitle(_title); };
}