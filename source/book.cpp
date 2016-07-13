// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include "gui.h" // includes book.h
#include "tinyxml2/tinyxml2.h"
#include "TextVisitor.h"
#include "rendering.h"

using namespace tinyxml2;

std::string get_extension(const std::string& filename)
{
    return filename.substr(filename.find_last_of(".")); 
}

template<typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

double degrees_to_radians(double degrees)
{
	return degrees * M_PI / 180.0;
}

Book::~Book()
{
	manifest.clear();
	spine.clear();
	alltext.clear();
}

void Book::CloseBook()
{
	manifest.clear();
	spine.clear();
	alltext.clear();

	book = "";
	opf = "";	
	page = 1;
	bookmark = 1;

	curVectorPos = 0;
	oldCurVectorPos = 0;
	oldCurVectorPos2 = 0;
	pageOld = 0;
}

void Book::LoadBook(const std::string& epub)
{
	book = epub;
	BLUnZip zp(book);

	ParseContainer(zp);
	ParseOPF(zp);
	ParsePages(zp);
}

void Book::ParseContainer(BLUnZip& zipfile)
{
	std::string unclean( zipfile.ExtractToString("META-INF/container.xml") );

	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(BLUnZip& zipfile)
{
	std::string unclean( zipfile.ExtractToString( opf ) );
	
	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* package = doc.FirstChildElement("package");
    XMLElement* manifest_ = package->FirstChildElement("manifest");
    XMLElement* item = manifest_->FirstChildElement("item");

	for(XMLElement* rfe = item; rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
	{
    	manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
	}

	XMLElement* spine_ = package->FirstChildElement("spine");
	XMLElement* itemref = spine_->FirstChildElement("itemref");

	for (XMLElement* rfe = itemref; rfe != nullptr; rfe = rfe->NextSiblingElement("itemref"))
	{
		spine.push_back(rfe->Attribute("idref"));
	}
}

void Book::ParsePages(BLUnZip& zipfile)
{
	// spine.size();
	for (unsigned int i = 0; i != 7; i++)
	{
		TextVisitor tv;

		std::string page ( zipfile.ExtractToString( manifest[spine[i]]) );

		XMLDocument doc;
		doc.Parse(page.c_str());
		XMLElement* body = doc.FirstChildElement("html")->FirstChildElement("body");
		body->Accept(&tv);

		for (auto& v : tv.GetText())
		{
			alltext.push_back(v);
		}
	}
}

std::string Book::GetBook()
{
	return book;
}

void Book::Reader(Gui& gui, Renderer& ren)
{	
	bool drawing = true;
	int pageNew = gui.getBookPage();

	// If we change page, draw the new text.
	if (pageOld != pageNew)
	{
		pageOld = pageNew;

		// draw top screen
		ren.StartDrawingTop();
		gui.DrawTextBG();

		int ypos = 20;
		int yposAddition = 0;
		int doNotUse = 0;
		while (drawing)
		{
			std::string textToDraw = alltext[curVectorPos];

			sftd_calc_bounding_box(&doNotUse, &yposAddition, gui.getFont(), 12, 400, textToDraw.c_str());
			sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, textToDraw.c_str());

			ypos += (15 + yposAddition);	

			if (ypos >= 200)
			{
				drawing = false;
			}
			else
			{
				curVectorPos++;
				if (curVectorPos == alltext.size())
				{
					curVectorPos--;
				}
			}
		}

		gui.DrawStatusScreen();
		ren.StopDrawing();

		// draw bottom screen
		ren.StartDrawingBottom();

		ypos = 20;
		yposAddition = 0;
		doNotUse = 0;
		while (drawing)
		{
			std::string textToDraw = alltext[curVectorPos];

			sftd_calc_bounding_box(&doNotUse, &yposAddition, gui.getFont(), 12, 400, textToDraw.c_str());
			sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, textToDraw.c_str());

			ypos += (15 + yposAddition);	

			if (ypos >= 180)
			{
				drawing = false;
			}
			else
			{
				curVectorPos++;
				if (curVectorPos == alltext.size())
				{
					curVectorPos--;
				}
			}
		}
		
		gui.DrawControls();
		ren.StopDrawing();

		oldCurVectorPos = curVectorPos;
		oldCurVectorPos2 = curVectorPos;
	}
	else
	{
		oldCurVectorPos = oldCurVectorPos2;
		// draw top screen
		ren.StartDrawingTop();
		gui.DrawTextBG();

		int ypos = 20;
		int yposAddition = 0;
		int doNotUse = 0;
		while (drawing)
		{
			std::string textToDraw = alltext[oldCurVectorPos];

			sftd_calc_bounding_box(&doNotUse, &yposAddition, gui.getFont(), 12, 400, textToDraw.c_str());
			sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, textToDraw.c_str());

			ypos += (15 + yposAddition);	

			if (ypos >= 200)
			{
				drawing = false;
			}
			else
			{
				oldCurVectorPos++;
				if (oldCurVectorPos == alltext.size())
				{
					oldCurVectorPos--;
				}
			}
		}

		gui.DrawStatusScreen();
		ren.StopDrawing();

		// draw bottom screen
		ren.StartDrawingBottom();

		ypos = 20;
		yposAddition = 0;
		doNotUse = 0;
		while (drawing)
		{
			std::string textToDraw = alltext[oldCurVectorPos];

			sftd_calc_bounding_box(&doNotUse, &yposAddition, gui.getFont(), 12, 400, textToDraw.c_str());
			sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, textToDraw.c_str());

			ypos += (15 + yposAddition);	

			if (ypos >= 180)
			{
				drawing = false;
			}
			else
			{
				oldCurVectorPos++;
				if (oldCurVectorPos == alltext.size())
				{
					oldCurVectorPos--;
				}
			}
		}
		
		gui.DrawControls();
		ren.StopDrawing();
	}
}	