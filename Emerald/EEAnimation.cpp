#include "EEAnimation.h"
#include "EECore.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEAnimation
	//----------------------------------------------------------------------------------------------------
	EEAnimation::EEAnimation()
		:
		EEObject(),
		m_frames(),
		m_startTime((float)EECore::s_EECore->GetTotalTime()),
		m_isLoop(false)
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Update()
	{
		if (!EEObject::Update())
			return false;

		float deltaTime = (float)EECore::s_EECore->GetTotalTime() - m_startTime;
		if (!m_frames.empty())
		{
			// find the current one
			EEAnimationFrame *frame = m_frames.front();
			while (frame && deltaTime >= frame->duration)
			{
				deltaTime -= frame->duration;
				m_startTime += frame->duration;
				m_frames.pop();
				SAFE_DELETE(frame);
				if (!m_frames.empty())
					frame = m_frames.front();
			}

			// update
			if (frame)
			{
				if (!frame->isRunning)
				{
					for (std::function<boost::thread*(EEObject*, float)>& action : frame->actions)
					{
						action(frame->object, -deltaTime);
					}
					frame->isRunning = true;
				}
				return frame->object->Update();
			}
		}
		else if (m_isLoop)
		{
			for (EEAnimationFrame* frame : m_backup)
			{
				m_frames.push(new EEAnimationFrame(*frame));
			}
		}
		else
		{
			SetIsAlive(false);
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Render()
	{
		if (!EEObject::Render())
			return false;

		if (!m_frames.empty())
		{
			return m_frames.front()->object->Render();
		}


		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Start()
	{
		// set to start
		SetIsAlive(true);
		m_startTime = (float)EECore::s_EECore->GetTotalTime();

		for (EEAnimationFrame* frame : m_backup)
		{
			m_frames.push(new EEAnimationFrame(*frame));
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::AddFrame(const EEAnimationFrame& _frame)
	{
		if (0 <= _frame.id && _frame.id < (int)m_backup.size())
		{
			m_backup.insert(m_backup.begin() + _frame.id, new EEAnimationFrame(_frame));
			m_backup[_frame.id]->object->SetParent(this);
			for (unsigned i = _frame.id + 1; i < m_backup.size(); ++i)
			{
				m_backup[i]->id = i;
			}
		}
		else
		{
			m_backup.push_back(new EEAnimationFrame(_frame));
			m_backup.back()->id = m_backup.size() - 1;
			m_backup.back()->object->SetParent(this);
		}

		return true;
	}
}