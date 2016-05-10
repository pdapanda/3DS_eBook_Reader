// (c) 2016 AlbertoSonic & reworks
// convert to xml to parse with tinyxml2.
// only worry about paragraphs - by spitting text when getting <div> tag
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

#include <algorithm>

#include "BLUnZip.h"
#include "tinyxml2.h"
#include "tidy.h"

#include "book.h"

using namespace tinyxml2;

char TidyBook(const std::string& buffer)
{
	char output;

	TidyDoc tdoc = tidyCreate();
	tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
	tidyParseString(tdoc, buffer.c_str());
	tidyCleanAndRepair(tdoc);

	tidySaveString(tdoc, &output, nullptr);

	return output;
}

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
	BLUnZip * zipfile = new BLUnZip( book );
	zipfile->List();

	std::string buffer( zipfile->ExtractToString( "META-INF/container.xml" ));
	buffer = TidyBook(buffer);

	delete zipfile;

	XMLDocument doc;
    doc.Parse( buffer.c_str() );

    XMLElement* rootfileElement = doc.FirstChildElement( "container" )->FirstChildElement( "rootfiles" )->FirstChildElement( "rootfile" );

    opf = rootfileElement->Attribute("full-path");
}

void Book::ParseOPF()
{
	BLUnZip * zipfile = new BLUnZip( book );
	zipfile->List();

	std::string buffer( zipfile->ExtractToString( opf ));
	buffer = TidyBook(buffer);

	delete zipfile;

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