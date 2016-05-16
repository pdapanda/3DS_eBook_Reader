// (c) 2016 AlbertoSonic & reworks

#include <unistd.h>
#include <sf2d.h>
#include <sftd.h>

#include "app.h"

#define MENU 0
#define TEXT 1
#define TO_MICROSECONDS 1000
#define WAIT_MILLISECONDS 50

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

	input = new Input;
	gui = new Gui;
	ren = new Renderer;

	input->curMode = MENU;
}

void App::Event()
{
	input->HandleEvents();

	// delay program to allow 3ds hardware to catch up.
	usleep(WAIT_MILLISECONDS*TO_MICROSECONDS);

	if (input->m_kDown & KEY_START) {
		input->running = false;
	}

	switch (input->curMode)
	{
		case MENU:
			gui->HandleEventsMenu(input);	
		break;

		case TEXT:
			gui->HandleEventsBook(input);
		break;
	}
}

void App::Update()
{
	gui->Update();
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

		case TEXT:
			ren->StartDrawingTop();

				gui->DrawBook();
				gui->DrawStatusScreen();
				
			ren->StopDrawing();
			ren->StartDrawingBottom();

				gui->DrawControls();

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

	ptmuExit();
	hidExit();
	aptExit();

	sf2d_fini();
	sftd_fini();
}