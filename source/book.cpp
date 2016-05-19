// (c) 2016 reworks
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>

#include "BLUnZip.h"
#include "tinyxml2.h"
#include "litehtml.h"
#include "book.h"

#include "container_3ds.h"

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

void Book::Reader()
{
	litehtml::context ctx;
	ctx.load_master_stylesheet(manifest["css"].c_str());
	
	container_3ds c3ds;

	litehtml::document::ptr doc;
	// = litehtml::document::createFromUTF8(manifest[spine[0]].c_str(), &c3ds, &ctx, 0);
	doc = litehtml::document::createFromUTF8("", &c3ds, &ctx);

	doc->render(400);
	//doc->draw(nullptr, 20, 20, nullptr);
}
