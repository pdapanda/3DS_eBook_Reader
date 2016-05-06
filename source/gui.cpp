// (c) 2016 AlbertoSonic & reworks

#include <sfil.h>

#include <cmath>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <dirent.h> 

#include "gui.h"
#include "input.h"

Gui::Gui()
{
	m_Font = sftd_load_font_file("res/Roboto-Regular.ttf");
	m_Next = sfil_load_PNG_file("res/NextFM.png", SF2D_PLACE_RAM);
	m_Prev = sfil_load_PNG_file("res/PrevFM.png", SF2D_PLACE_RAM);
	m_Top = sfil_load_PNG_file("res/top.png", SF2D_PLACE_RAM);
	m_Bottom = sfil_load_PNG_file("res/bottom.png", SF2D_PLACE_RAM);
	m_Exit = sfil_load_PNG_file("res/exit.png", SF2D_PLACE_RAM);
	m_Charging = sfil_load_PNG_file("res/BatteryCharge.png", SF2D_PLACE_RAM);

	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryEmpty.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryLowest.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryLow.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryMid.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryHigh.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("res/BatteryFull.png", SF2D_PLACE_RAM));

	DIR *dir;
	struct dirent *ent;
	
	dir = opendir("books");

	// print all the files and directories within directory
	while ((ent = readdir (dir)) != NULL) {
	    files.push_back(ent->d_name);
	}
	
	closedir (dir);
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
	for(auto& v : m_BatteryLevels) {
		sf2d_free_texture(v);
	}
}

void Gui::HandleEvents(Input* input)
{
	if (input->m_kDown & KEY_UP) { m_Index--; }
	if (input->m_kDown & KEY_DOWN) { m_Index++; }
	if (input->m_kDown & KEY_LEFT) { m_curPage--; m_Index = 0; }
	if (input->m_kDown & KEY_RIGHT) { m_curPage++; m_Index = 0; }
	
	// correct values
	if (m_Index < 0) m_Index = 0;
	if (m_Index > 6) m_Index = 6;
	if (m_curPage < 0) m_curPage = 0;

	if (input->m_kDown & KEY_A) { 
		selected = files[m_Index+(7*m_curPage)]; 
		input->curMode = 1;
	}

	if (input->m_PosX >= 0 && input->m_PosX <= 320 && input->m_PosY >= 217 && input->m_PosY <= 241) {
		input->running = false;
	}

	if (input->m_PosX >= 295 && input->m_PosX <= 320 && input->m_PosY >= 65 && input->m_PosY <= 137) {
		m_curPage++; 
		m_Index = 0;
	}

	if (input->m_PosX >= 9 && input->m_PosX <= 31 && input->m_PosY >= 65 && input->m_PosY <= 137) {
		m_curPage--; 
		m_Index = 0;
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

std::string Gui::getSelected()
{
	return selected;
}

void Gui::DrawTopBackground()
{
	sf2d_draw_texture(m_Top, 0, 0);
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
    sftd_draw_text (m_Font, 20, 2, RGBA8(0, 0, 0, 255), 12, remove_extension(selected).c_str());
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

// Thanks to http://stackoverflow.com/a/6417908
std::string Gui::remove_extension(const std::string& filename) {
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

/*

void GUI_OptionsUi(bool greyScale)
{
	gfxDrawSprite(GFX_BOTTOM, GFX_LEFT, (u8*)Options_bin, 171, 291, 14, 46);

	if (greyScale) //Color selection
	{
		drawFillRect(54, 176, 144, 184, 192, 192, 192, screenBottom);
	}
	else
	{
		drawFillRect(172, 176, 263, 184, 192, 192, 192, screenBottom);
	}
}

void GUI_TextView(char* path, int page, bool greyScale)
{
	//Draw BKG
	gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)TextBKG_bin, 240, 400, 0, 0); 
	if (greyScale)
	{
		drawFillRect(40, 20, 359, 239, 255, 255, 255, screenTopLeft);
		drawFillRect(0, 0, 319, 239, 255, 255, 255, screenBottom);
	}
	else
	{
		drawFillRect(40, 20, 359, 239, 243, 231, 189, screenTopLeft);
		drawFillRect(0, 0, 319, 239, 243, 231, 189, screenBottom);
	}

	//Draw page
	sprintf(buffer, "PG: %i", page + 1);
	gfxDrawText(GFX_TOP, GFX_LEFT, &fontBlack, buffer, 5, 238 - fontBlack.height * 15);	

	//Read the page
	int c;
	FILE *file;
	nextAvailable = true;
	
	file = fopen(path, "r");
	if (file) {
		//Show bookmark (with animation)
		if (page == TextView_GetBookmark(path))
		{
			if (bookmark_animation != 24) bookmark_animation+=4;
		}
		else bookmark_animation = 0;
		if(bookmark_animation != 0) drawFillRect(368, 20, 385, 20 + bookmark_animation, 255, 0, 0, screenTopLeft);

		//Start reading the page
        fseek(file, !page ? 4 : offsets[page], SEEK_SET);
		int i = 0;
		for (i = 0; i < 29; i++)
		{
			//Loads the line
			int j = 0;
			char line[55];

			for (j = 0; j < 55; j++)
			{
				if ((c = getc(file)) != EOF && c != '\n') line[j] = c;
				else
				{
					if (c == EOF) nextAvailable = false;
					line[j] = '\0';
					break;
				}
			}

			//Prints text
			gfxDrawText(
				i < 14 ? GFX_TOP : GFX_BOTTOM, GFX_LEFT,                 //Screen on which to print the text
				greyScale ? &fontBlack : &fontBrown,
				line,                                                    //Line of text to print
				i < 14 ? 44 : 5,                                         //Horizzontal position of the text
				238 - fontBlack.height * (i < 14 ? i + 2 : i + 1 - 14)       //Vertical position of the text
				);
		}
        //Save the current offset in file, for every page
	    fflush(file);
	    offsets[page+1] = ftell(file);
		fclose(file);
	}
	else
	{
		//Error
	    
	}

	//Next/Previous Page buttons:
	if (page!=0) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)PrevPG_bin, 71, 23, 0, 85);       // <--
	if (nextAvailable) gfxDrawSprite(GFX_TOP, GFX_LEFT, (u8*)NextPG_bin, 72, 22, 378, 85); // -->	

}
*/