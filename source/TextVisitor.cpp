#include "TextVisitor.h"	

bool TextVisitor::VisitEnter(const XMLDocument& doc)
{
	return true;
}

bool TextVisitor::VisitExit(const XMLDocument& doc)
{
	return true;
}

bool TextVisitor::VisitEnter(const XMLElement& element, const XMLAttribute* attribute)
{
	return true;
}

bool TextVisitor::VisitExit(const XMLElement& element)
{
	return true;
}

bool TextVisitor::Visit(const XMLDeclaration& declaration)
{
	return true;
}

bool TextVisitor::Visit(const XMLText& text)
{
	bookText.push_back(text.Value());
	
	return true;
}

bool TextVisitor::Visit(const XMLComment& comment)
{
	return true;
}

bool TextVisitor::Visit(const XMLUnknown& unknown)
{
	return true;
}

std::vector<std::string> TextVisitor::GetText()
{
	return bookText;
}