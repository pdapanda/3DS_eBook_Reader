#ifndef RENDERING_H
#define RENDERING_H

#include "container_3ds.h"

class Renderer
{
public:
	Renderer();

	void StartDrawingTop();
	void StartDrawingBottom();
	void StopDrawing();
	void Render();

	litehtml::context ctx;
	litehtml::uint_ptr hdc = nullptr;

	container_3ds c3ds;
};

#endif