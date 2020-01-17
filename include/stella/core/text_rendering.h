#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H 
#include <vector>
#include <map>
#include "stella/graphics/opengl.h"
#include <glm/glm.hpp>

namespace stella
{
namespace core
{

struct Character {
  GLuint     TextureID;  // ID handle of the glyph texture
  glm::ivec2 Size;       // Size of glyph
  glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
  GLuint     Advance;    // Offset to advance to next glyph
};

class TextRendering {
  public:
    TextRendering();
    ~TextRendering();

  private:
    FT_Library m_ft;
    FT_Face m_face;
    std::map<GLchar, Character> m_chars;
};

}
}
