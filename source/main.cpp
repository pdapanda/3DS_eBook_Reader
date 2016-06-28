#include "app.h"

int main()
{
	App app;

	app.Init();

	while (aptMainLoop())
	{
		while (app.input.running)
		{
			app.Event();
			app.Update();
			app.Render();
		}
	}
	
	app.End();

	return 0;
}