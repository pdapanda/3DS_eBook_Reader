// (c) reworks 2016.
// Implementation of litehtml for Nintendo 3DS using ctrulib.

#include "container_3ds.h"

#include <3ds.h>
#include <sftd.h>

// Freetype portlib. requirement of sftd. will help us with other font actions

#include <freetype2/	ft2build.h>
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

void container_3ds::delete_font(litehtml::uint_ptr hFont)
{

}

int container_3ds::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{

}

void container_3ds::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{

}

int container_3ds::pt_to_px(int pt)
{
	
}

int container_3ds::get_default_font_size() const
{

}

const litehtml::tchar_t* container_3ds::get_default_font_name() const
{

}

void container_3ds::load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready)
{

}

void container_3ds::get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz)
{

}

void container_3ds::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg)
{

}

void container_3ds::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{

}

void container_3ds::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{

}

std::shared_ptr<litehtml::element>	container_3ds::create_element(const litehtml::tchar_t *tag_name, const litehtml::string_map &attributes, const std::shared_ptr<litehtml::document> &doc)
{

}

void container_3ds::get_media_features(litehtml::media_features& media) const
{

}

void container_3ds::get_language(litehtml::tstring& language, litehtml::tstring & culture) const
{

}

void container_3ds::link(const std::shared_ptr<litehtml::document> &ptr, const litehtml::element::ptr& el)
{

}

void container_3ds::transform_text(litehtml::tstring& text, litehtml::text_transform tt)
{

}

void container_3ds::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y)
{

}

void container_3ds::del_clip()
{

}