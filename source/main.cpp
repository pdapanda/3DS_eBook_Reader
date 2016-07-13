#include "app.h"

int main()
{
	App app;

	app.Init();

	while (app.input.running)
	{
		aptMainLoop();
		
		app.Event();
		app.Update();
		app.Render();
	}
	
	app.End();

	return 0;
}