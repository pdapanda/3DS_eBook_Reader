#include <sfil.h>

#include <cmath>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <dirent.h> 

#include "gui.h"
#include "input.h"

// Thanks to http://stackoverflow.com/a/6417908
std::string Gui::remove_extension(const std::string& filename)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

Gui::~Gui()
{
	sftd_free_font(m_Font);
	sf2d_free_texture(m_Next);
	sf2d_free_texture(m_Prev);
	sf2d_free_texture(m_Top);
	sf2d_free_texture(m_Bottom);
	sf2d_free_texture(m_Exit);
	sf2d_free_texture(m_Charging);
	sf2d_free_texture(m_About);
	sf2d_free_texture(m_Controls);
	sf2d_free_texture(m_TextBG);

	for(auto& v : m_BatteryLevels) {
		sf2d_free_texture(v);
	}
}

void Gui::Load()
{
	m_Font = sftd_load_font_file("res/font/LiberationSans-Regular.ttf");
	m_Next = sfil_load_PNG_file("res/NextFM.png", SF2D_PLACE_RAM);
	m_Prev = sfil_load_PNG_file("res/PrevFM.png", SF2D_PLACE_RAM);
	m_Top = sfil_load_PNG_file("res/top.png", SF2D_PLACE_RAM);
	m_Bottom = sfil_load_PNG_file("res/bottom.png", SF2D_PLACE_RAM);
	m_Exit = sfil_load_PNG_file("res/exit.png", SF2D_PLACE_RAM);
	m_Charging = sfil_load_PNG_file("res/BatteryCharge.png", SF2D_PLACE_RAM);
	m_About = sfil_load_PNG_file("res/about.png", SF2D_PLACE_RAM);
	m_Controls = sfil_load_PNG_file("res/controls.png", SF2D_PLACE_RAM);
	m_TextBG = sfil_load_PNG_file("res/text.png", SF2D_PLACE_RAM);

	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryEmpty.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryLowest.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryLow.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryMid.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryHigh.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryFull.png", SF2D_PLACE_RAM));

	DIR *dir;
	struct dirent *ent;
	
	dir = opendir("books");

	while ((ent = readdir (dir)) != nullptr	)
	{
	    files.push_back(ent->d_name);
	}
	
	closedir (dir);
}

void Gui::HandleEventsMenu(Input& input)
{
	if (loading)
	{
		if (selected != "")
		{
			OpenBook(selected);
			loading = false;
			input.curMode = 1;
		}
	}

	if (input.m_kDown & KEY_UP) { m_Index--; }
	if (input.m_kDown & KEY_DOWN) { m_Index++; }
	if (input.m_kDown & KEY_LEFT) { m_curPage--; m_Index = 0; }
	if (input.m_kDown & KEY_RIGHT) { m_curPage++; m_Index = 0; }
	
	// correct values
	if (m_Index < 0) m_Index = 0;
	if (m_Index > 6) m_Index = 6;
	if (m_curPage < 0) m_curPage = 0;

	if (input.m_kDown & KEY_X) { RemoveBook(files[m_Index+(7*m_curPage)]); }

	if (input.m_kDown & KEY_A) { 
		selected = files[m_Index+(7*m_curPage)]; 
		if (selected != "") {
			loading = true;
		}
	}

	if (input.m_PosX >= 159 && input.m_PosX <= 320 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		if (drawAbout == true) {
			drawAbout = false;
		} else {
			drawAbout = true;
		}
	}

	if (input.m_PosX >= 295 && input.m_PosX <= 320 && input.m_PosY >= 65 && input.m_PosY <= 137) {
		m_curPage++; 
		m_Index = 0;
	}

	if (input.m_PosX >= 9 && input.m_PosX <= 31 && input.m_PosY >= 65 && input.m_PosY <= 137) {
		m_curPage--; 
		m_Index = 0;
	}

	if (input.m_PosX >= 0 && input.m_PosX <= 158 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		input.running = false;
	}
}

void Gui::HandleEventsBook(Input& input)
{
	if (input.m_kDown & KEY_LEFT) { m_BookPage--; }
	if (input.m_kDown & KEY_RIGHT) { m_BookPage++; }

	if (m_BookPage < 1) { m_BookPage = 1; }

	if (input.m_PosX >= 0 && input.m_PosX <= 99 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		input.curMode = 0;
		CloseBook();
	}

	if (input.m_PosX >= 101 && input.m_PosX <= 221 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		// bookmark page
	}	
}

void Gui::Update()
{
	PTMU_GetBatteryLevel(&batteryLevel);
	PTMU_GetBatteryChargeState(&charging);
}

void Gui::DrawFileSelect()
{
	sf2d_draw_texture(m_Bottom, 0, 0);
	sf2d_draw_texture(m_Exit, 0, 217);

	if (!loading)
	{
		int pos = 20;
	
		if (m_curPage == 0) {
			begin = 0;
			end = 7; 
		} else {
			begin = (7*m_curPage);
			end = (7*m_curPage) + 6;
		}

		if (end > files.size()) {
			end = files.size();
		}

		for (begin = begin; begin < end; ++begin) {
			sftd_draw_text(m_Font, 57, pos, RGBA8(0, 0, 0 ,255), 12, files[begin].c_str());
			pos += 20;
		}

		sftd_draw_text(m_Font, 45, (m_Index + 1) * 20, RGBA8(0, 0, 0, 255), 12, "->");
		sf2d_draw_texture(m_Next, 295, 65);

		if (m_curPage > 0) {
			sf2d_draw_texture(m_Prev, 9, 65);
		}
	}
	else
	{
		sftd_draw_text(m_Font, (320 / 2) - ( sftd_get_text_width(m_Font, 12, "Loading...") / 2), 100, RGBA8(0, 0, 0, 255), 12, "Loading...");
	}
}

std::string Gui::getSelected()
{
	return selected;
}

void Gui::DrawTopBackground()
{
	if (drawAbout) {
		sf2d_draw_texture(m_About, 0, 0);
	} else {
		sf2d_draw_texture(m_Top, 0, 0);
	}
}

void Gui::DrawStatusScreen()
{
	// Battery icon
	if (charging)
		sf2d_draw_texture(m_Charging, 0, 0);
	else 
		sf2d_draw_texture(m_BatteryLevels[batteryLevel], 0, 0);

	// clock
	sftd_draw_text (m_Font, 299, 2, RGBA8(0, 0, 0, 255), 12, clock().c_str());

	// Title - Name of the book!
    if (!drawAbout) {
    	sftd_draw_text (m_Font, 20, 2, RGBA8(0, 0, 0, 255), 12, remove_extension(selected).c_str());
    }
}

void Gui::OpenBook(const std::string& bookName)
{
	std::string fullBook = "books/"+bookName;

	book.LoadBook(fullBook.c_str());
}

void Gui::CloseBook()
{
	book.CloseBook();
}

void Gui::DrawBook(Gui& gui)
{
	sf2d_draw_texture(m_TextBG, 0, 0);

	book.Reader(gui);
}

void Gui::DrawControls()
{
	sf2d_draw_texture(m_Bottom, 0, 0);
	sf2d_draw_texture(m_Controls, 0, 217);
}

std::string Gui::clock()
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string temp = std::ctime(&time);
	
	// Remove pesky newlines
	temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

	// Trim string info down to MON JAN 01 HH:MM format
	temp = temp.substr(0, 16);

	return temp;
}

void Gui::RemoveBook(const std::string& file)
{
	// remove book from sd card
	std::string path = "books/" + file;
	remove(path.c_str());

	// erase book from vector
	files.erase(std::find(files.begin(), files.end(), file));
	files.shrink_to_fit();
}