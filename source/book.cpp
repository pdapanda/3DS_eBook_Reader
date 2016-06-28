// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>

#include "book.h"
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
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(lastdot, 4); 
}

Book::~Book()
{
	manifest.clear();
	spine.clear();
	content.clear();
}

void Book::CloseBook(Renderer& ren)
{
	manifest.clear();
	spine.clear();
	content.clear();

	book = "";
	opf = "";	
}

void Book::LoadBook(const std::string& epub, Renderer& ren)
{
	book = epub;
	BLUnZip zp(book);

	ParseContainer(zp);
	ParseOPF(zp);
	ParsePages(zp);

	std::string test = "<html><head></head><body><p>Hello world!</p><p>Hi!</p></body></html>";
	//std::string test = zipfile->ExtractToString(manifest["css"]);

	doc = litehtml::document::createFromUTF8(test.c_str(), &ren.c3ds, &ren.ctx);
}

void Book::ParseContainer(BLUnZip& zipfile)
{
	std::string unclean ( zipfile.ExtractToString("META-INF/container.xml") );

	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(BLUnZip& zipfile)
{
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

void Book::ParsePages(BLUnZip& zipfile)
{
	for (unsigned int i = 0; i < spine.size(); ++i)
	{
		if (get_extension(manifest[spine[i]]) == ".html") 
		{
			std::string text = zipfile.ExtractToString(manifest[spine[i]].c_str());
			content.push_back(text);
		}
	}
}

std::string Book::GetBook()
{
	return book;
}

void Book::Reader(Renderer& ren)
{
	doc->render(400);
	doc->draw(&ren.hdc, 0, 20, nullptr);
}