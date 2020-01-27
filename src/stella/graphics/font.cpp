#include "stella/graphics/font.h"

#include <algorithm>

#include <glm/glm.hpp>

namespace stella
{
namespace graphics
{
  Font::Font (const std::string& path, std::size_t size) : Resource (path), m_path (path.c_str()), m_size (size)
  {
    if (FT_Init_FreeType (&m_ft))
    {
      std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }
    if (FT_New_Face (m_ft, m_path, 0, &m_face))
    {
      std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    if (FT_Select_Charmap (m_face, FT_ENCODING_UNICODE))
    {
      std::cout << "ERROR::FREETYPE: Failed to load unicode charmap" << std::endl;
    }
    FT_Set_Pixel_Sizes (m_face, 0, m_size);

    unsigned int aw = 0, ah = 0;
    for (wchar_t c = CHAR_BOTTOM_LIMIT; c < CHAR_TOP_LIMIT; c++)
    {
      if (FT_Load_Char (m_face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      aw += m_face->glyph->bitmap.width;
      ah = std::max (ah, m_face->glyph->bitmap.rows);
    }

    m_atlas_width  = aw;
    m_atlas_height = ah;

    m_texture_atlas = new Texture{aw, ah};

    int aa = 0;
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    m_texture_atlas->Bind();

    for (wchar_t c = CHAR_BOTTOM_LIMIT; c < CHAR_TOP_LIMIT; c++)
    {
      if (FT_Load_Char (m_face, c, FT_LOAD_RENDER))
      {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
      }
      glTexSubImage2D (GL_TEXTURE_2D,
                       0,
                       aa,
                       0,
                       m_face->glyph->bitmap.width,
                       m_face->glyph->bitmap.rows,
                       GL_RED,
                       GL_UNSIGNED_BYTE,
                       m_face->glyph->bitmap.buffer);

      CharacterData ch_data = {m_face->glyph->advance.x,
                               m_face->glyph->advance.y,
                               m_face->glyph->bitmap.width,
                               m_face->glyph->bitmap.rows,
                               m_face->glyph->bitmap_left,
                               m_face->glyph->bitmap_top,
                               (float) aa / m_atlas_width};
      m_chars.insert (std::pair<wchar_t, CharacterData> (c, ch_data));
      aa += m_face->glyph->bitmap.width;
    }
    m_texture_atlas->Unbind();

    FT_Done_Face (m_face);
    FT_Done_FreeType (m_ft);
  }

  Font::~Font() { delete m_texture_atlas; }

} // namespace graphics
} // namespace stella
