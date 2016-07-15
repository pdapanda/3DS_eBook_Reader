#include "rendering.h"

#include <sf2d.h>

void Renderer::StartDrawingTop()
{
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
}

void Renderer::StartDrawingTopRight()
{
	sf2d_start_frame(GFX_TOP, GFX_RIGHT);
}

void Renderer::StartDrawingBottom()
{
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
}

void Renderer::StopDrawing()
{
	sf2d_end_frame();
}

void Renderer::Render()
{
	sf2d_swapbuffers();
}