// (c) 2016 AlbertoSonic & reworks
// convert to xml to parse with tinyxml2.
// only worry about paragraphs - by spitting text when getting <div> tag
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

#include <algorithm>

#include "zipper/unzipper.h"
#include "tinyxml2/tinyxml2.h"
#include "tidybook.h"
#include "book.h"

using namespace zipper;
using namespace tinyxml2;

Book::~Book()
{
	manifest.clear();
	spine.clear();
}

void Book::LoadBook(const std::string& epub)
{
	book = epub;

	ParseContainer();
	ParseOPF();
}

void Book::ParseContainer()
{
	Unzipper unzipper(book);

	std::vector<unsigned char> temp;

	unzipper.extractEntryToMemory("META-INF/container.xml", temp);

	char tempArray[temp.size()];
	std::copy(temp.begin(), temp.end(), tempArray);

	std::string buffer (tempArray);
	buffer = TidyBook(buffer);

	unzipper.close();

	XMLDocument doc;
    doc.Parse( buffer.c_str() );

    XMLElement* rootfileElement = doc.FirstChildElement( "container" )->FirstChildElement( "rootfiles" )->FirstChildElement( "rootfile" );

    opf = rootfileElement->Attribute("full-path");
}

void Book::ParseOPF()
{
	Unzipper unzipper(book);

	std::vector<unsigned char> temp;

	unzipper.extractEntryToMemory(opf, temp);

	char tempArray[temp.size()];
	std::copy(temp.begin(), temp.end(), tempArray);

	std::string buffer (tempArray);
	buffer = TidyBook(buffer);

	unzipper.close();

	XMLDocument doc;
    doc.Parse( buffer.c_str() );

	for(XMLElement* rfe = doc.FirstChildElement("package")->FirstChildElement("manifest")->FirstChildElement("item"); rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
	{
    	manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
	}

	for (XMLElement* rfe = doc.FirstChildElement("package")->FirstChildElement("spine"); rfe != nullptr; rfe = rfe->NextSiblingElement("itemref"))
	{
		spine.push_back(rfe->Attribute("idref"));
	}
}

void Book::ParseText()
{
	
}