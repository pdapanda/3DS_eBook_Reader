// (c) reworks

#ifndef TIDYBOOK_H
#define TIDYBOOK_H

#include <string>

#include "tidy/tidy.h"

static char TidyBook(const std::string& buffer)
{
	char output;

	TidyDoc tdoc = tidyCreate();
	tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
	tidyParseString(tdoc, buffer.c_str());
	tidyCleanAndRepair(tdoc);

	tidySaveString(tdoc, &output, nullptr);

	return output;
}

#endif