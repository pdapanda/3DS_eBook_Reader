#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <vector>
#include <string>
#include <unordered_map>

#include "zip/BLUnZip.h"

// Forward dec
class Gui;

class Book
{
public:
	~Book();

	void LoadBook(const std::string& epub);

	void ParseContainer(BLUnZip& zipfile);
	void ParseOPF(BLUnZip& zipfile);
	void ParsePages(BLUnZip& zipfile);

	std::string GetBook();

	void Reader(Gui& gui);
	void CloseBook();
private:
	std::string book;
	std::string opf;

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;

	// contents of pages
	std::vector<std::string> content;
};

#endif