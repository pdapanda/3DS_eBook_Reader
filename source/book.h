// (c) 2016 AlbertoSonic & reworks

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <vector>
#include <string>
#include <unordered_map>

class Book
{
public:
	~Book();
	void LoadBook(const std::string& epub);

	void ParseContainer();
	void ParseOPF();

	std::string GetBook();
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