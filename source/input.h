#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>

class Input
{
public:
	void HandleEvents();

private:
	touchPosition touch;

public:
	u16 m_PosX;
	u16 m_PosY;
	u32 m_kDown;
	bool running = true;
	int curMode = 0;
};

#endif