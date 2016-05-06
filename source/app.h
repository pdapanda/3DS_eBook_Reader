// (c) 2016 AlbertoSonic & reworks

#ifndef APP_H
#define APP_H

#include "gui.h"
#include "input.h"
#include "rendering.h"
#include "textview.h"

class App
{
public:
	void Init();

	void Event();
	void Update();
	void Render();

	void End();

public:
	Input* input;
	Renderer* ren;
	Gui* gui;
	TextView* book;
};

#endif