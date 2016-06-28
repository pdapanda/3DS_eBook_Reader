#include "rendering.h"

#include <sf2d.h>
#include <string>
#include <cstdio>

Renderer::Renderer()
{
	FILE* fp = fopen("res/master.css", "r");
	fseek(fp, 0L, SEEK_END);
	long bufsize = ftell(fp);

	char* buffer[bufsize+1];
	fseek(fp, 0L, SEEK_SET);

	size_t newLen = fread(buffer, sizeof(char), bufsize, fp);
	buffer[newLen++] = '\0';

	std::string wut((const char*)buffer);
	
	ctx.load_master_stylesheet(wut.c_str());

	fclose(fp);
}

void Renderer::StartDrawingTop()
{
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
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