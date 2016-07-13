#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <unordered_map>

#include "zip/BLUnZip.h"

// Forward dec
class Gui;
class Renderer;

class Book
{
public:
	~Book();

	void LoadBook(const std::string& epub);

	void ParseContainer(BLUnZip& zipfile);
	void ParseOPF(BLUnZip& zipfile);
	void ParsePages(BLUnZip& zipfile);

	void Reader(Gui& gui, Renderer& ren);
	void CloseBook();

	std::string GetBook();
private:
	std::string book = "";
	std::string opf = "";
	
	int page = 1;
	int bookmark = 1;

	unsigned int curVectorPos = 0;
	unsigned int oldCurVectorPos = 0;
	unsigned int oldCurVectorPos2 = 0;
	int pageOld = 0;

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;

	// All the text in the book.
	std::vector<std::string> alltext;

};

#endif