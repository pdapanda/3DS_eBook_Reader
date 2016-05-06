// (c) 2016 AlbertoSonic & reworks

#ifndef TEXTVIEW_H
#define TEXTVIEW_H

#include <string>
#include <vector>

class TextView
{
public:
	~TextView();

	void Parse(const std::string& book);


private:
	std::vector<unsigned char> m_BookBuffer;
	std::string opf;
};

#endif