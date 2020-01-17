#include "stella/core/text_rendering.h"

#include <iostream>

namespace stella
{
namespace core
{

TextRendering::TextRendering()
{
  if (FT_Init_FreeType(&m_ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
  }
  if (FT_New_Face(m_ft, "assets/fonts/1980.ttf", 0, &m_face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  }
  FT_Set_Pixel_Sizes(m_face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
    
  for (GLubyte c = 0; c < 128; c++)
  {
    // Load character glyph 
    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        m_face->glyph->bitmap.width,
        m_face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        m_face->glyph->bitmap.buffer
    );
    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Now store character for later use
    Character character =
    {
        texture, 
        glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
        glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
        m_face->glyph->advance.x
    };
    m_chars.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(m_face);
  FT_Done_FreeType(m_ft);
}

TextRendering::~TextRendering()
{

}

}
}
