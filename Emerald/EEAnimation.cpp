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
		m_backup(),
		m_startTime((float)EECore::s_EECore->GetTotalTime()),
		m_isLoop(false)
	{

	}
	
	//----------------------------------------------------------------------------------------------------
	EEAnimation::EEAnimation(const EEAnimation& _animation)
		:
		EEObject(_animation),
		m_frames(),
		m_backup(),
		m_startTime(_animation.m_startTime),
		m_isLoop(_animation.m_isLoop)
	{
		for (EEAnimationFrame* frame : _animation.m_backup)
		{
			m_backup.push_back(new EEAnimationFrame(*frame));
			m_backup.back()->object->SetParent(this);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEAnimation::~EEAnimation()
	{
		while (!m_frames.empty())
		{
			EEAnimationFrame* frame = m_frames.front();
			SAFE_DELETE(frame);
			m_frames.pop();
		}
		for (EEAnimationFrame* frame : m_backup)
		{
			SAFE_DELETE(frame);
		}
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
					for (auto& action : frame->actions)
					{
						action(frame->object, -deltaTime);
					}
					frame->isRunning = true;
				}
				return frame->object->Update();
			}
		}
		// reset
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
			if (m_frames.front()->isRunning)
				return m_frames.front()->object->Render();
		}


		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Start()
	{
		if (IsAlive() && !m_frames.empty())
			return false;

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

	//----------------------------------------------------------------------------------------------------
	void EEAnimation::SetIsLoop(bool _isLoop)
	{
		m_isLoop = _isLoop;
	}

	// EEAnimationEmitter
	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::EEAnimationEmitter()
		:
		m_animations(),
		m_backup(nullptr)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::EEAnimationEmitter(const EEAnimationEmitter& _emitter)
		:
		m_animations(),
		m_backup(new EEAnimation(*_emitter.m_backup))
	{

	}
	
	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::~EEAnimationEmitter()
	{
		for (EEAnimation* animation : m_animations)
		{
			SAFE_DELETE(animation);
		}
		SAFE_DELETE(m_backup);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimationEmitter::Update()
	{
		if (!EEObject::Update())
			return false;

		for (EEAnimation* animation : m_animations)
		{
			animation->Update();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimationEmitter::Render()
	{
		if (!EEObject::Render())
			return false;

		for (EEAnimation* animation : m_animations)
		{
			animation->Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimationEmitter::Emit(const FLOAT3& _pos)
	{
		if (m_backup)
		{
			bool flag = false;
			for (EEAnimation* animation : m_animations)
			{
				if (!animation->IsAlive())
				{
					animation->SetPosition(_pos);
					animation->Start();
					flag = true;
					break;
				}
			}

			if (!flag)
			{
				m_animations.push_back(new EEAnimation(*m_backup));
				EEAnimation* animation = m_animations.back();
				animation->SetPosition(_pos);
				animation->Start();
			}

			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimationEmitter::SetAnimation(const EEAnimation& _animation)
	{
		SAFE_DELETE(m_backup);

		m_backup = new EEAnimation(_animation);

		return true;
	}
}