#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <vector>
#include <string>
#include <unordered_map>

#include "rendering.h"
#include "zip/BLUnZip.h"

class Book
{
public:
	~Book();

	void LoadBook(const std::string& epub, Renderer& ren);

	void ParseContainer(BLUnZip& zipfile);
	void ParseOPF(BLUnZip& zipfile);
	void ParsePages(BLUnZip& zipfile);

	std::string GetBook();

	void Reader(Renderer& ren);
	void CloseBook(Renderer& ren);
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