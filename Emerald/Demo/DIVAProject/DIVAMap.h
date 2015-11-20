#pragma once


//----------------------------------------------------------------------------------------------------
class DIVAMap
{
	//----------------------------------------------------------------------------------------------------
	enum NOTE_TYPE { NOTETYPE_NORMAL = 0, NOTETYPE_STRIP = 1 };
	enum NOTE_KEY { NOTE_CIRCLE = 0, NOTE_RECT = 1, NOTE_CROSS = 2, NOTE_TRIANGLE = 3, NOTE_RIGHT = 4, NOTE_LEFT = 5, NOTE_DOWN = 6, NOTE_UP = 7 };

	//----------------------------------------------------------------------------------------------------
	class Note
	{
	public:
		inline Note() : notePos(0), sound(0), x(0), y(0), tailX(0), tailY(0), key(0) {}

	public:
		int notePos;
		int sound;
		int key;
		int  x, y;
		float tailX, tailY;
		float duration;
	};

	//----------------------------------------------------------------------------------------------------
public:
	DIVAMap();
	virtual ~DIVAMap();


private:

};