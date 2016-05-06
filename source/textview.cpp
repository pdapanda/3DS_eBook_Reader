// (c) 2016 AlbertoSonic & reworks

#include <vector>
#include <algorithm>

#include "zipper/unzipper.h"
#include "tinyxml2/tinyxml2.h"
#include "tidy/tidy.h"

#include "textview.h"

using namespace zipper;
using namespace tinyxml2;

TextView::~TextView()
{

}

void TextView::Parse(const std::string& book)
{
	Unzipper unzipper(book);

	std::vector<unsigned char> temp;
	unzipper.extractEntryToMemory("META-INF/container.xml", temp);

	char containerBuffer[temp.size()];
	std::copy(temp.begin(), temp.end(), (char*)containerBuffer);

	XMLDocument doc;
    doc.Parse( containerBuffer );

    XMLElement* rootfileElement = doc.FirstChildElement( "container" )->FirstChildElement( "rootfiles" )->FirstChildElement( "rootfile" );

    opf = rootfileElement->Attribute("full-path");

    // ensure vector is completely empty...
    temp.clear();
    temp.shrink_to_fit();
    temp.clear();

	unzipper.extractEntryToMemory(opf, temp);    

	char opfBuffer[temp.size()];
	std::copy(temp.begin(), temp.end(), (char*)opfBuffer);

	XMLDocument opfXML;
	opfXML.Parse(opfBuffer);





	unzipper.close();
}

// convert to xml to parse with tinyxml2.
// only worry about paragraphs - by spitting text when getting <div> tag
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html