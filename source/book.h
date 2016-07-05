#ifndef BOOK_H
#define BOOK_H

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

	void Reader(Gui& gui);
	void CloseBook();

	std::string GetBook();
private:
	std::string book;
	std::string opf;

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;

	// Text in page with spine as key
	std::unordered_map<std::string, std::vector<std::string>> content;

};

#endif