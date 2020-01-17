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

class Font 
{
  public:
    Font(const char *path, unsigned size = 48);
    ~Font();
    CharacterData get_char_data(GLchar c) {return m_chars[c];};
    inline Texture *get_atlas() const {return m_texture_atlas;};
  private:
    const char *m_path;
    unsigned m_size = 12;
    FT_Library m_ft;
    FT_Face m_face;
    std::map<GLchar, CharacterData> m_chars;
    Texture *m_texture_atlas;
    unsigned int m_atlas_width, m_atlas_height;
};

}
}

