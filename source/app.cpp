// (c) 2016 AlbertoSonic & reworks

#include <3ds.h>

#include <unistd.h>

#include "app.h"

#define MENU 0
#define TEXT 1
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