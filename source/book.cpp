// (c) 2016 AlbertoSonic & reworks
// convert to xml to parse with tinyxml2.
// only worry about paragraphs - by spitting text when getting <div> tag
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>

#include "BLUnZip.h"
#include "tinyxml2.h"

#include "book.h"

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
	BLUnZip zipfile (book);
	std::string unclean ( zipfile.ExtractToString("META-INF/container.xml") );
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
	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF()
{
	BLUnZip zipfile ( book );
	
	std::string unclean( zipfile.ExtractToString( opf ));
	
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

std::string Book::GetBook()
{
	return book;
}

// iterate through <p> tags, store text using ->GetText(), then draw with text wrap and scrolling.
// mehbeh lock to 60ups?