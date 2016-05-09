// (c) 2016 AlbertoSonic & reworks

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <vector>
#include <string>
#include <unordered_map>

class Book
{
private:
	void ParseContainer();
	void ParseOPF();

public:
	~Book();
	void LoadBook(const std::string& epub);

private:
	std::string book;
	std::string opf;

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;
};

#endif