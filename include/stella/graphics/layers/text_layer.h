#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <vector>
#include <map>
#include "stella/graphics/opengl.h"
#include <glm/glm.hpp>

#include "./layer.h"
#include "../shader.h"
#include "stella/graphics/texture.h"

namespace stella {
namespace graphics {

struct Character {
  GLuint     TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;       // Size of glyph
  glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
  GLuint     Advance;    // Offset to advance to next glyph
  Texture    *Tex;
};

struct character_info {
  long int ax; // advance.x
  long int ay; // advance.y
  
  unsigned bw; // bitmap.width;
  unsigned bh; // bitmap.rows;
  
  int bl; // bitmap_left;
  int bt; // bitmap_top;
  
  float tx; // x offset of glyph in texture coordinates
};

class TextLayer : public Layer {
  public:
    TextLayer(float width, float height, bool fixed = true);
    ~TextLayer();
    void Render() override;
  private:
    FT_Library m_ft;
    FT_Face m_face;
    std::map<GLchar, Character> m_chars;
    std::map<GLchar, character_info> m_char_infos;
    Texture *m_texture_atlas;
    unsigned int m_atlas_width, m_atlas_height;
};
} // namespace graphics
} // namespace stella