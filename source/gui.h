// (c) 2016 AlbertoSonic & reworks

#ifndef GUI_H
#define GUI_H

#include <3ds.h>

#include <sf2d.h>
#include <sftd.h>

#include <vector>
#include <string>

class Input;

class Gui
{
public:
	Gui();
	~Gui();

	void HandleEvents(Input* input);
	void Update();

	// Top Screen
	void DrawTopBackground();
	void DrawStatusScreen();

	// Bottom Screen
	void DrawFileSelect();

	void Cleanup();

	std::string getSelected();

private:
	int m_Index = 0;
	int m_curPage = 0;
	unsigned int begin = 0;
	unsigned int end = 7;

	sf2d_texture* m_Next;
	sf2d_texture* m_Prev;
	sf2d_texture* m_Top;
	sf2d_texture* m_Bottom;
	sf2d_texture* m_Exit;
	std::vector<sf2d_texture*> m_BatteryLevels;
	sf2d_texture* m_Charging;

	sftd_font* m_Font;

	std::vector<std::string> files;
	std::string selected = "";

	u32 wifiStatus = 0;
	u8 batteryLevel = 5;
	u8 charging = 0;

private:
	std::string clock();
	std::string remove_extension(const std::string& filename);
};

#endif