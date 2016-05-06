// (c) 2016 AlbertoSonic & reworks

#include "app.h"

int main()
{
	App app;

	app.Init();

	while (aptMainLoop() && app.input->running)
	{
		app.Event();
		app.Update();
		app.Render();
	}

	app.End();

	return 0;
}