#ifndef RENDERING_H
#define RENDERING_H

class Renderer
{
public:
	void StartDrawingTop();
	void StartDrawingBottom();
	void StopDrawing();
	void Render();
};

#endif