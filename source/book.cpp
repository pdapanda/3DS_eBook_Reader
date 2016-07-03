// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>
#include <sstream>

#include <sftd.h>

#include "gui.h" // includes book.h
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

/*
	TidyBuffer output = {0};
	TidyDoc tdoc = tidyCreate();

	tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
	tidyParseString(tdoc, unclean.c_str());
	tidyCleanAndRepair(tdoc);
	tidyRunDiagnostics(tdoc);
	
	tidySaveBuffer(tdoc, &output);
	
	std::string clean(output.bp, output.bp + output.size);

    tidyBufFree(&output);
	tidyRelease(tdoc);
*/

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
		if(get_extension(manifest[spine[i]]) == ".html")
		{
			std::string page ( zipfile.ExtractToString( manifest[spine[i]]) );
			content.push_back(page);
		}
	}
}

std::string Book::GetBook()
{
	return book;
}

// loop through each element of body using nextsibling element
// parse each subtree using iterate() fuction to parse all nodes in that.
// keep going deeper until we exit.
void Book::Reader(Gui& gui)
{	
	int ypos = 20;

	std::string test = content[2];
	
	XMLDocument doc;
	doc.Parse(test.c_str());

	XMLElement* root = doc.FirstChildElement("html");

	//XMLText* textNode = root->FirstChildElement("body")->FirstChild()->ToText();
	//const char* text = textNode->Value();

	//sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, text);

	for (XMLElement* body = root->FirstChildElement("body"); body != nullptr; body = body->NextSibling())
	{
		std::string textFromElement = " ";
		XMLText* textNode = body->FirstChild()->ToText();
		if (textNode)
		{
			textFromElement = textNode->Value();
		}

		sftd_draw_text_wrap(gui.getFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, 400, textFromElement.c_str());
		ypos += 10;
	}
}