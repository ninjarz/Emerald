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
		startTime(-1.0f),
		m_isLoop(false)
	{

	}
	
	//----------------------------------------------------------------------------------------------------
	EEAnimation::EEAnimation(const EEAnimation& _animation)
		:
		EEObject(_animation),
		m_frames(),
		m_backup(),
		startTime(_animation.startTime),
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
		for (EEAnimationFrame* frame : m_frames)
			SAFE_DELETE(frame);
		m_frames.clear();
		for (EEAnimationFrame* frame : m_backup)
			SAFE_DELETE(frame);
		m_backup.clear();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Update()
	{
		if (!EEObject::Update())
			return false;

		if (startTime < 0.0f)
			startTime = (float)EECore::s_EECore->GetTotalTime();
		float deltaTime = (float)EECore::s_EECore->GetTotalTime() - startTime;
		if (!m_frames.empty())
		{
			for (auto it = m_frames.begin(); it != m_frames.end();)
			{
				EEAnimationFrame* frame = *it;
				if (frame->startTime + frame->duration < deltaTime)
				{
					SAFE_DELETE(frame);
					it = m_frames.erase(it);
					continue;
				}
				else if (frame->isRunning)
				{
					frame->object->Update();
				}
				else if (frame->startTime < deltaTime)
				{
					for (auto& action : frame->actions)
					{
						action(frame->object, frame->startTime - deltaTime);
					}
					frame->isRunning = true;
					frame->object->Update();
				}
				++it;
			}
		}
		// reset
		else if (m_isLoop)
		{
			Start();
		}
		else
			SetIsAlive(false);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Render()
	{
		if (!EEObject::Render())
			return false;

		for (EEAnimationFrame* frame : m_frames)
		{
			if (frame->isRunning)
				frame->object->Render();
		}


		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimation::Start()
	{
		// start
		SetIsAlive(true);
		startTime = (float)EECore::s_EECore->GetTotalTime();
		m_frames.clear();
		for (EEAnimationFrame* frame : m_backup)
		{
			m_frames.push_back(new EEAnimationFrame(*frame));
		}

		return true;
	}

	// new
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
	bool EEAnimation::RemoveFrame(int _id)
	{
		if (_id == -1)
		{
			for (EEAnimationFrame* frame : m_frames)
				SAFE_DELETE(frame);
			m_frames.clear();
			for (EEAnimationFrame* frame : m_backup)
				SAFE_DELETE(frame);
			m_backup.clear();
			return true;
		}
		else if (0 <= _id &&_id < (int)m_backup.size())
		{
			m_backup.erase(m_backup.begin() + _id);
			for (unsigned i = _id; i < m_backup.size(); ++i)
			{
				m_backup[i]->id = i;
			}
			return true;
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	void EEAnimation::SetIsLoop(bool _isLoop)
	{
		m_isLoop = _isLoop;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<EEAnimationFrame*> EEAnimation::GetFrames()
	{
		return m_backup;
	}

	// EEAnimationEmitter
	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::EEAnimationEmitter()
		:
		m_animations(),
		m_backup(new EEAnimation()),
		m_isAnimationDirty(false)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::EEAnimationEmitter(const EEAnimationEmitter& _emitter)
		:
		m_animations(),
		m_backup(new EEAnimation(*_emitter.m_backup)),
		m_isAnimationDirty(false)
	{

	}
	
	//----------------------------------------------------------------------------------------------------
	EEAnimationEmitter::~EEAnimationEmitter()
	{
		for (EEAnimation* animation : m_animations)
		{
			SAFE_DELETE(animation);
		}
		m_animations.clear();
		SAFE_DELETE(m_backup);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEAnimationEmitter::Update()
	{
		if (!EEObject::Update())
			return false;

		if (m_isAnimationDirty)
		{
			for (EEAnimation* animation : m_animations)
			{
				SAFE_DELETE(animation);
			}
			m_animations.clear();
			m_isAnimationDirty = false;
		}

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
	bool EEAnimationEmitter::SetAnimation(const EEAnimation* _animation)
	{
		SAFE_DELETE(m_backup);

		if (_animation)
			m_backup = new EEAnimation(*_animation);
		else
			m_backup = new EEAnimation();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	void EEAnimationEmitter::SetIsAnimationDirty(bool _isAnimationDirty)
	{
		m_isAnimationDirty = _isAnimationDirty;
	}

	//----------------------------------------------------------------------------------------------------
	EEAnimation* EEAnimationEmitter::GetAnimation()
	{
		return m_backup;
	}
}