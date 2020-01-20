#pragma once
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include "stella/graphics/texture.h"
#include "stella/graphics/opengl.h"

namespace stella
{
namespace graphics
{

struct CharacterData
{
  long int ax; // advance.x
  long int ay; // advance.y
  unsigned bw; // bitmap.width;
  unsigned bh; // bitmap.rows;
  int bl; // bitmap_left;
  int bt; // bitmap_top;
  float tx; // x offset of glyph in texture coordinates
};

class Font : public core::Resource
{
  public:
    Font(const std::string &path, unsigned size = 48);
    ~Font();
    CharacterData get_char_data(wchar_t c) {return m_chars[c];};
    inline Texture *get_atlas() const {return m_texture_atlas;};
  private:
    const char *m_path;
    unsigned m_size;
    FT_Library m_ft;
    FT_Face m_face;
    std::map<wchar_t, CharacterData> m_chars;
    Texture *m_texture_atlas;
    unsigned int m_atlas_width, m_atlas_height;

    // Limit codes for obtaining characters in the ttf font
    static const int CHAR_BOTTOM_LIMIT = 32;
    static const int CHAR_TOP_LIMIT = 253;
};

}
}

