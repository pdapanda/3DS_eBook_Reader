// (c) 2016 AlbertoSonic & reworks

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <vector>
#include <string>
#include <unordered_map>

#include "BLUnZip.h"

class Book
{
public:
	~Book();
	void LoadBook(const std::string& epub);

	void ParseContainer();
	void ParseOPF();
	void ParsePages();

	std::string GetBook();

	void Reader();
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

	BLUnZip* zipfile;
};

#endif