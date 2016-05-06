// (c) reworks 2016.
// Implementation of litehtml for Nintendo 3DS using ctrulib.

#include "container_3ds.h"

#include <3ds.h>
#include <sftd.h>

// Freetype portlib. requirement of sftd. will help us with other font actions

#include <ft2build.h>
#include FT_CACHE_H
#include FT_FREETYPE_H

container_3ds::container_3ds(void)
{
}

container_3ds::~container_3ds(void)
{
}

litehtml::uint_ptr container_3ds::create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm)
{
	sftd_font* font = sftd_load_font_file(faceName);
	

	FTC_Manager_LookupFace(font->ftcmanager, face_id, &face);
}