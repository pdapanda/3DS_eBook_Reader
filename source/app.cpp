#include <unistd.h>
#include <sf2d.h>
#include <sftd.h>

#include "app.h"

#define MENU 0
#define TEXT 1

int MilliToMicro(int millis)
{
	return millis * 1000;
}

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

	gui.Load();

	input.curMode = MENU;
}

void App::Event()
{
	input.HandleEvents();

	// delay program to allow 3ds hardware to catch up.
	usleep(MilliToMicro(50));

	if (input.m_kDown & KEY_START) {
		input.running = false;
	}

	switch (input.curMode)
	{
		case MENU:
			gui.HandleEventsMenu(input);	
		break;

		case TEXT:
			gui.HandleEventsBook(input);
		break;
	}
}

void App::Update()
{
	gui.Update();
}

void App::Render()
{
	switch (input.curMode)
	{
		case MENU:
			ren.StartDrawingTop();

				gui.DrawTopBackground();
				gui.DrawStatusScreen();
				
			ren.StopDrawing();
			ren.StartDrawingBottom();

				gui.DrawFileSelect();

			ren.StopDrawing();
			ren.Render();
		break;

		case TEXT:
			// all calls are in this
			gui.DrawBook(gui, ren);
				
			ren.Render();
		break;
	}
}

void App::End()
{
	gui.Close();

	sftd_fini();
	sf2d_fini();

	hidExit();
	ptmuExit();
	aptExit();
}