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
	void Load();
	void Close();

	void HandleEventsMenu(Input& input);
	void HandleEventsBook(Input& input);
	void Update();

	// Top Screen
	void DrawTopBackground();
	void DrawStatusScreen();
	void DrawTextBG();

	// Bottom Screen
	void DrawFileSelect();

	void OpenBook(const std::string& bookName);
	void CloseBook();

	// Top Screen
	void DrawBook(Gui& gui);

	// Bottom Screen
	void DrawControls();

	std::string getSelected();

	sftd_font* getTextFont();

	int getBookPage();

private:
	Book book;

	int m_Index = 0;
	int m_curPage = 0;
	int m_BookPage = 0;
	unsigned int begin = 0;
	unsigned int end = 7;
	bool drawAbout = false;
	bool loading = false;
	bool bookmarked = false;

	sf2d_texture* m_Next;
	sf2d_texture* m_Prev;
	sf2d_texture* m_Top;
	sf2d_texture* m_Bottom;
	sf2d_texture* m_Exit;
	sf2d_texture* m_About;
	sf2d_texture* m_Controls;
	sf2d_texture* m_Charging;
	sf2d_texture* m_TextBG;
	sf2d_texture* m_BookmarkedBG;
	std::vector<sf2d_texture*> m_BatteryLevels;

	sftd_font* m_Font;
	sftd_font* m_TextFont;

	std::vector<std::string> files;
	std::string selected = "";

	u32 wifiStatus = 0;
	u8 batteryLevel = 5;
	u8 charging = 0;

private:
	std::string clock();
	std::string remove_extension(const std::string& filename);
	
	void RemoveBook(const std::string& file);
	void LoadBookmark();
	void SaveBookmark();
};

#endif