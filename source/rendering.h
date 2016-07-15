#ifndef RENDERING_H
#define RENDERING_H

class Renderer
{
public:
	void StartDrawingTop();
	void StartDrawingTopRight();
	void StartDrawingBottom();
	void StopDrawing();
	void Render();

	bool draw3D = false;
};

#endif