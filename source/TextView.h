// (c) 2016 AlbertoSonic & reworks

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <string>
#include <unordered_map>


class TextView
{
public:
	~TextView();

	void Parse(const std::string& book);


private:
	std::unordered_map<std::string, std::string> manifest;
};

#endif