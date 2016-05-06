// (c) 2016 AlbertoSonic & reworks

#include <3ds.h>

#include <unistd.h>

#include "app.h"

#define MENU 0
#define TEXT 1
#define EXIT 2
#define TO_MICROSECONDS 1000

void App::Init()
{
	// Initialize services
	aptInit();
	ptmuInit();
	hidInit();

	sf2d_init();
	sf2d_set_clear_color(RGBA8(0, 0, 0, 255));
	sf2d_set_vblank_wait(0);
	
	sftd_init();

	input->curMode = MENU;

	input = new Input;
	gui = new Gui;
	ren = new Renderer;
	book = new TextView;
}

void App::Event()
{
	input->HandleEvents();

	// to allow events to proccess on 3ds hardware...
	usleep(50*TO_MICROSECONDS);

	if (input->m_kDown & KEY_START) {
		input->running = false;
	}

	switch (input->curMode)
	{
		case MENU:
			gui->HandleEvents(input);	
		break;
	}
}

void App::Update()
{
	switch (input->curMode)
	{
		case MENU:
			gui->Update();
		break;
	}
}

void App::Render()
{
	switch (input->curMode)
	{
		case MENU:
			ren->StartDrawingTop();

			gui->DrawTopBackground();
			gui->DrawStatusScreen();
				
			ren->StopDrawing();
			ren->StartDrawingBottom();

			gui->DrawFileSelect();

			ren->StopDrawing();
			ren->Render();
		break;
	}
}

void App::End()
{
	delete input;
	delete gui;
	delete ren;
	delete book;

	ptmuExit();
	hidExit();
	aptExit();

	sf2d_fini();
	sftd_fini();
}

//Text screen
//TextView_Main();
//GUI_TextView(path, page, greyScale);
//GUI_StatusBar(title);
//if (!hideUI)GUI_QuickUi();
//if (!hideOptions)GUI_OptionsUi(greyScale);