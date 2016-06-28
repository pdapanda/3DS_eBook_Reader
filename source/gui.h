#ifndef GUI_H
#define GUI_H

#include <3ds.h>

#include <sf2d.h>
#include <sftd.h>

#include "book.h"

class Input;

class Gui
{
public:
	~Gui();

	void Load();

	void HandleEventsMenu(Input& input, Renderer& ren);
	void HandleEventsBook(Input& input, Renderer& ren);
	void Update();

	// Top Screen
	void DrawTopBackground();
	void DrawStatusScreen();

	// Bottom Screen
	void DrawFileSelect();

	void OpenBook(const std::string& bookName, Renderer& ren);
	void CloseBook(Renderer& ren);

	// Top Screen
	void DrawBook(Renderer& ren);

	// Bottom Screen
	void DrawControls();

	std::string getSelected();

private:
	int m_Index = 0;
	int m_curPage = 0;
	int m_BookPage = 1;
	unsigned int begin = 0;
	unsigned int end = 7;
	bool drawAbout = false;

	sf2d_texture* m_Next;
	sf2d_texture* m_Prev;
	sf2d_texture* m_Top;
	sf2d_texture* m_Bottom;
	sf2d_texture* m_Exit;
	sf2d_texture* m_About;
	std::vector<sf2d_texture*> m_BatteryLevels;
	sf2d_texture* m_Charging;
	sf2d_texture* m_Controls;
	sf2d_texture* m_TextBG;

	sftd_font* m_Font;

	std::vector<std::string> files;
	std::string selected = "";

	u32 wifiStatus = 0;
	u8 batteryLevel = 5;
	u8 charging = 0;

private:
	std::string clock();
	std::string remove_extension(const std::string& filename);
	void RemoveBook(const std::string& file);

	Book book;
};

#endif