#include "DIVAHelper.h"
#if _DIVA_
#include "DIVAMana.h"
#include "DIVAConfig.h"

DIVANote::DIVANote(const Note& _note, double _totalTime, double singleTime, EETexture& _nodeTex, EETexture& _stripTex)
	:
	EEQuad2D(FLOAT3((float)_note.x, (float)_note.y, 0.f), EEGetWidth() / 20.f, EEGetHeight() / 11.f, _nodeTex),

	m_type(_note.key < NOTE_NUM ? NOTETYPE_NORMAL : NOTETYPE_STRIP),
	m_key(_note.key),
	m_x(_note.x),
	m_y(_note.y),
	m_tailx(_note.tailx),
	m_taily(_note.taily),
	m_note(FLOAT3((float)_note.tailx, (float)_note.taily, 0.f), EEGetWidth() / 20.f, EEGetHeight() / 11.f, _nodeTex),
	m_stripNote(FLOAT3((float)_note.tailx, (float)_note.taily, 0.f), EEGetWidth() / 20.f, EEGetHeight() / 11.f, _nodeTex),
	m_strip(_stripTex),

	m_state(DIVA_NOTE_DEFAULT),
	m_actionTime(0.2),
	m_totalTime(_totalTime),
	m_restTime(_totalTime),

	m_totalDuration(_note.duration * singleTime),
	m_restDuration(_note.duration * singleTime)
{
	m_strip.SetWidth(EEGetWidth() / 30.f);
}

bool DIVANote::Update(double _deltaTime)
{
	m_restTime -= _deltaTime;

	// Normal
	if (m_type == NOTETYPE_NORMAL)
	{
		// Hit time
		if (abs(m_restTime) <= m_actionTime && m_state == DIVA_NOTE_DEFAULT)
		{
			if (EEIsKeyInput())
			{
				unsigned int key = EEPeekKey();
				if (DIVAConfig::GetKeyMap(m_key) == key)
				{
					EEGetKey();
					m_state = DIVA_NOTE_COOL;
				}
				// wrong key
				else
					m_state = DIVA_NOTE_WORST;
			}
		}
		// Missed
		else if (m_restTime + m_actionTime <= 0 && m_state == DIVA_NOTE_DEFAULT)
		{
			m_state = DIVA_NOTE_WORST;
		}

		float percent = (float)(m_totalTime - m_restTime) / m_totalTime;
		m_note.SetPosition(FLOAT3(percent * (m_x - m_tailx) + m_tailx, percent * (m_y - m_taily) + m_taily, 0.f));

		Process();
		m_strip.Process();
		m_note.Process();
	}
	// Strip
	else
	{
		if (m_restTime < 0)
		{
			m_restDuration = m_totalDuration + m_restTime;
		}

		// Hit time
		if (abs(m_restTime) <= m_actionTime && m_state == DIVA_NOTE_DEFAULT)
		{
			if (EEIsKeyInput())
			{
				unsigned int key = EEPeekKey();
				if (DIVAConfig::GetKeyMap(m_key) == key)
				{
					EEGetKey();
					m_state = DIVA_NOTE_COOL;
				}
				// wrong key
				else
					m_state = DIVA_NOTE_WORST;
			}
		}
		// Missed
		else if (m_restTime + m_actionTime <= 0 && m_state == DIVA_NOTE_DEFAULT)
		{
			m_state = DIVA_NOTE_WORST;
		}
		// Strip time
		else if (m_restTime < 0 && m_state != DIVA_NOTE_DEFAULT)
		{
			if (!EEIsKeyDown(DIVAConfig::GetKeyMap(m_key)))
				m_state = DIVA_NOTE_STRIP_WORST;
		}
		// Release time
		else if (abs(m_restDuration) <= m_actionTime && m_state != DIVA_NOTE_DEFAULT)
		{
			if (!EEIsKeyDown(DIVAConfig::GetKeyMap(m_key)))
				m_state = DIVA_NOTE_STRIP_COOL;
		}
		// Missed
		else if (m_restDuration < 0)
		{
			if (EEIsKeyDown(DIVAConfig::GetKeyMap(m_key)))
				m_state = DIVA_NOTE_STRIP_WORST;
		}

		float percent = (float)(m_totalTime - m_restTime) / m_totalTime;
		FLOAT2 vertical = (FLOAT2(m_x, m_y) - FLOAT2(m_tailx, m_taily)).GetVertical() * 30.f;
		FLOAT2 center1 = (FLOAT2(m_x, m_y) - FLOAT2(m_tailx, m_taily)) / 3.f + FLOAT2(m_tailx, m_taily) - vertical;
		FLOAT2 center2 = (FLOAT2(m_x, m_y) - FLOAT2(m_tailx, m_taily)) * 2 / 3.f + FLOAT2(m_tailx, m_taily) + vertical;
		std::vector<FLOAT2> pointsData;
		for (float t = percent - m_restDuration / m_totalTime; t < percent; t += 0.001f)
			pointsData.push_back(EEBezier(FLOAT2(m_tailx, m_taily), center1, center2, FLOAT2(m_x, m_y), t));

		if (pointsData.size())
		{
			m_stripNote.SetPositionXY(pointsData.front());
			m_note.SetPositionXY(pointsData.back());
		}
		else
			m_note.SetPositionXY(FLOAT2(m_x, m_y));
		m_strip.SetCurve(pointsData);

		Process();
		m_strip.Process();
		m_note.Process();
		m_stripNote.Process();
	}

	return true;
}


//----------------------------------------------------------------------------------------------------
DIVAMana::DIVAMana(wchar_t* _fileName)
	:
	m_isStart(false),
	m_currentTime(0.f),
	m_totalTime(0.f),
	m_noteTime(0.f),
	m_noteTimeForward(0.f),
	m_singleTime(0.f),
	m_distanceTime(0.f),

	m_noteMap(_fileName),
	m_notes(),
	m_emitter(),

	circleTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal/Normal-Circle.png"),
	crossTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal/Normal-Cross.png"),
	squareTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal/Normal-Square.png"),
	triangleTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal/Normal-Triangle.png"),
	rightTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal-Array/Normal-Array Right.png"),
	leftTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal-Array/Normal-Array Left.png"),
	downTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal-Array/Normal-Array Down.png"),
	upTex(L"Texture/Project Diva Freedom/自由模式/NOTE/NOTE/Normal-Array/Normal-Array Up.png"),
	stripBlueTex(L"Texture/Project Diva Freedom/自由模式/NOTE/Strip/Strip/Strip-Blue.png"),
	stripGreenTex(L"Texture/Project Diva Freedom/自由模式/NOTE/Strip/Strip/Strip-Green.png"),
	stripPinkTex(L"Texture/Project Diva Freedom/自由模式/NOTE/Strip/Strip/Strip-Pink.png"),
	stripRedTex(L"Texture/Project Diva Freedom/自由模式/NOTE/Strip/Strip/Strip-Red.png")
{
	EETexture hitTex[34] = {
		L"Texture/Project Diva Freedom/自由模式/HIT/effect30.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect29.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect28.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect27.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect26.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect25.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect24.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect23.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect22.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect21.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect20.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect19.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect18.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect17.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect16.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect15.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect14.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect13.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect12.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect11.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect10.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect9.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect8.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect7.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect6.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect5.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect4.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect3.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/effect2.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-1（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-2（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-3（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/hit-4（线性减淡）.png",
		L"Texture/Project Diva Freedom/自由模式/HIT/sad-01.png",
	};
	// 0
	EESlide hitSlide(Rect_Float(-64, -64, 64, 64), hitTex, 29, 0.0f, false);
	hitSlide.SetInterval(0.05f / 29.f);

	EEAnimationFrame frame0;
	frame0.SetObject(hitSlide);
	frame0.startTime = 0.0f;
	frame0.duration = 0.05f;

	// 1
	EEQuad2D quad1(Rect_Float(-40, -40, 40, 40), hitTex[29]);
	//quad1.SetScale(3.0f);
	EEAnimationFrame frame1;
	frame1.SetObject(quad1);
	frame1.startTime = 0.15f;
	frame1.duration = 0.35f;
	float alpha11 = 0.0f;
	float scale11 = 2.0f;
	frame1.actions.push_back(boost::bind(EEFade, _1, 0.35f, alpha11, _2));
	frame1.actions.push_back(boost::bind(EEScale, _1, 0.35f, scale11, _2));

	// 2
	EEQuad2D quad2(Rect_Float(-40, -40, 40, 40), hitTex[30]);
	EEAnimationFrame frame2;
	frame2.SetObject(quad2);
	frame2.startTime = 0.05f;
	frame2.duration = 0.05f;
	float rotation2 = EE_2PI;
	frame2.actions.push_back(boost::bind(EERotateYX, _1, 0.05f, rotation2, _2, false));

	// 3
	EEQuad2D quad3(Rect_Float(-50, -50, 50, 50), hitTex[31]);
	float alpha30 = 0.8f;
	float alpha31 = 0.0f;
	quad3.SetAlpha(alpha30);
	EEAnimationFrame frame3;
	frame3.SetObject(quad3);
	frame3.startTime = 0.1f;
	frame3.duration = 0.05f;
	frame3.actions.push_back(boost::bind(EEFade, _1, 0.05f, alpha31, _2));

	// 4
	EEQuad2D quad4(Rect_Float(-50, -50, 50, 50), hitTex[32]);
	EEAnimationFrame frame4;
	frame4.SetObject(quad4);
	frame4.startTime = 0.5f;
	frame4.duration = 0.07f;
	float alpha41 = 0.0f;
	float scale41 = 4.0f;
	frame4.actions.push_back(boost::bind(EEFade, _1, 0.07f, alpha41, _2));
	frame4.actions.push_back(boost::bind(EEScale, _1, 0.07f, scale41, _2));

	EEAnimation animation;
	animation.AddFrame(frame0);
	animation.AddFrame(frame2);
	animation.AddFrame(frame3);
	animation.AddFrame(frame1);
	animation.AddFrame(frame4);
	//animation.SetIsLoop(true);

	m_emitter.SetAnimation(&animation);
}

DIVAMana::~DIVAMana()
{
	for (auto it = m_music.begin(); it != m_music.end(); ++it)
		SAFE_DELETE(it->second);
}

bool DIVAMana::Start()
{
	// load music files
	auto music = m_noteMap.GetMusicPaths();
	for (auto it = music.begin(); it != music.end(); ++it)
	{
		m_music[it->first] = new EEMusic();
		m_music[it->first]->AsyncLoadMusic(it->second.c_str());
		m_music[it->first]->SetVolume(0.5);
	}

	m_startTime = EEGetTotalTime();
	m_isStart = true;
	return true;
}

bool DIVAMana::Process()
{
	if (m_isStart)
	{
		double currentTime = EEGetTotalTime() - m_startTime;
		double deltaTime = currentTime - m_currentTime;
		m_currentTime = currentTime;

		// handle all notes
		for (auto ptr = m_notes.begin(); ptr != m_notes.end(); )
		{
			ptr->Update(deltaTime);
			// Normal
			if (ptr->m_type == NOTETYPE_NORMAL)
			{
				switch (ptr->m_state)
				{
				case DIVA_NOTE_DEFAULT:
					break;
				case DIVA_NOTE_COOL:
					m_emitter.Emit(ptr->GetPosition());
					ptr = m_notes.erase(ptr);
					continue;
				case DIVA_NOTE_FINE:
					break;
				case DIVA_NOTE_SAD:
					break;
				case DIVA_NOTE_SAFE:
					break;
				case DIVA_NOTE_WORST:
					ptr = m_notes.erase(ptr);
					continue;
				default:
					ptr = m_notes.erase(ptr);
					continue;
				}
			}
			// strip
			else
			{
				switch (ptr->m_state)
				{
				case DIVA_NOTE_DEFAULT:
					break;
				case DIVA_NOTE_COOL:
					m_emitter.Emit(ptr->GetPosition());
					continue;
				case DIVA_NOTE_FINE:
					break;
				case DIVA_NOTE_SAD:
					break;
				case DIVA_NOTE_SAFE:
					break;
				case DIVA_NOTE_WORST:
					ptr = m_notes.erase(ptr);
					continue;
				case DIVA_NOTE_STRIP_COOL:
					m_emitter.Emit(ptr->GetPosition());
					ptr = m_notes.erase(ptr);
					continue;
				case DIVA_NOTE_STRIP_FINE:
					break;
				case DIVA_NOTE_STRIP_SAD:
					break;
				case DIVA_NOTE_STRIP_SAFE:
					break;
				case DIVA_NOTE_STRIP_WORST:
					ptr = m_notes.erase(ptr);
					continue;
				default:
					ptr = m_notes.erase(ptr);
					continue;
				}
			}

			++ptr;
		}
		EEClearKey();

		// handle new frames
		while (m_noteTime <= m_currentTime)
		{
			Frame* frame = m_noteMap.GetFrame();
			if (frame)
			{
				if (frame->isSetBPM)
				{
					m_singleTime = frame->GetSingleTime();
				}
				// load files, play music
				for (int i = 0; i < BGS_NUM; i++)
				{
					if (frame->music[i] >= 0)
						m_music[frame->music[i]]->Start();
				}
				
				m_noteTime = frame->timePos;
			}
			else
				break;
		}

		// handle notes forward (192 frames)
		while (true)
		{
			Frame* frame = m_noteMap.GetFrameForward();
			if (frame)
			{
				for (int i = 0; i < frame->noteNum; ++i)
				{
					switch (frame->notes[i].key)
					{
					case 0:
					case 8:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, circleTex, stripBlueTex));
						break;
					case 1:
					case 9:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, squareTex, stripBlueTex));
						break;
					case 2:
					case 10:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, crossTex, stripBlueTex));
						break;
					case 3:
					case 11:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, triangleTex, stripBlueTex));
						break;
					case 4:
					case 12:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, rightTex, stripBlueTex));
						break;
					case 5:
					case 13:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, leftTex, stripBlueTex));
						break;
					case 6:
					case 14:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, downTex, stripBlueTex));
						break;
					case 7:
					case 15:
						m_notes.push_back(DIVANote(frame->notes[i], frame->timePos - currentTime, m_singleTime, upTex, stripBlueTex));
						break;
					}
				}
				m_noteTimeForward = frame->timePos;
			}
			else
				break;
		}

		m_emitter.Process();
	}

	return true;
}

#endif