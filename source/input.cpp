#include "input.h"

void Input::HandleEvents()
{
	hidScanInput();
	m_kDown = hidKeysDown();

	hidTouchRead(&touch);

	m_PosX = touch.px;
	m_PosY = touch.py;
}