// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cmath>

#include <sftd.h>

#include "gui.h" // includes book.h
#include "tinyxml2/tinyxml2.h"
#include "TextVisitor.h"

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
	content.clear();
}

void Book::CloseBook()
{
	manifest.clear();
	spine.clear();
	content.clear();

	book = "";
	opf = "";	
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
	for (unsigned int i = 0; i != 5; i++)
	{
		TextVisitor tv;

		std::string page ( zipfile.ExtractToString( manifest[spine[i]]) );

		XMLDocument doc;
		doc.Parse(page.c_str());
		XMLElement* body = doc.FirstChildElement("html")->FirstChildElement("body");
		body->Accept(&tv);

		content.emplace(spine[i], tv.GetText());
	}
}

std::string Book::GetBook()
{
	return book;
}

void Book::Reader(Gui& gui)
{	
	int ypos = 20;
	unsigned int curVectorPos = 0;
	bool drawing = true;

	sf2d_rendertarget* target = sf2d_create_rendertarget(400, 240);

	sf2d_start_frame_target(target);
 
	while (drawing)
	{
		std::string textToDraw = content[spine[curVectorPos]];

		ypos += 15;
		curVectorPos++;
	}

	for (unsigned int i = 0; i < content[spine[3]].size(); i++)
	{
		std::string test = content[spine[3]][i];
		sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, test.c_str());
		ypos += 15;

		if (ypos >= 230)
		{
			break;
		}
	}

	sf2d_end_frame();
	sf2d_texture_tile32(target->texture);

	// gotta draw sideways.
	// draw on top screen then bottom

	sf2d_draw_texture_rotate(target->texture, 0, 0, degrees_to_radians(90.0));
}

void Book::SaveBookmark(point pt)
{
	std::string bookmarkFile = "bookmarks/" + book + ".bookmark";
	std::ofstream bookmarkStream;

	bookmarkStream.open(bookmarkFile);

	bookmarkStream << pt.x << std::endl;
	bookmarkStream << pt.y << std::endl;
	bookmarkStream << pt.z << std::endl;

	bookmarkStream.close();
}

point Book::ReadBookmark()
{
	point temp;
	temp.x = temp.y = temp.z = 0;

	std::string bookmarkFile = "bookmarks/" + book + ".bookmark";
	std::ifstream bookmarkStream;
	
	if (bookmarkStream.open(bookmarkFile))
	{
		bookmarkStream >> temp.x >> std::endl;
		bookmarkStream >> temp.y >> std::endl;
		bookmarkStream >> temp.z >> std::endl;
	}

	bookmarkStream.close();

	return temp;
}