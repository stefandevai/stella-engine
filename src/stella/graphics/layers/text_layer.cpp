#include <algorithm>

#include "stella/graphics/layers/text_layer.h"
#include "stella/graphics/texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace stella {
namespace graphics {
TextLayer::TextLayer(float width, float height, bool fixed)
  : Layer(std::shared_ptr<Renderer>(new Renderer()), fixed) {
    // Initialize shader and textures IDs
    this->Shad = std::shared_ptr<Shader>(new Shader("assets/shaders/sprite_batch.vert", "assets/shaders/text.frag"));
    GLint tex_ids[21] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    this->Shad->Enable();
    const auto projection = glm::ortho(0.0f, width, height, 0.0f, -20.0f, 10.0f);
    this->Shad->SetMat4("proj", projection);
    this->Shad->SetIntv("textures", tex_ids, 21);
    this->Shad->Disable();

  if (FT_Init_FreeType(&m_ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
  }
  if (FT_New_Face(m_ft, "assets/fonts/1980.ttf", 0, &m_face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
  }
  FT_Set_Pixel_Sizes(m_face, 0, 48);
  
  // BEGIN ATLAS

  unsigned int aw = 0, ah = 0;
  for (GLubyte c = 32; c < 128; c++)
  {
    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    aw += m_face->glyph->bitmap.width;
    ah = std::max(ah, m_face->glyph->bitmap.rows);  
  }
  
  m_atlas_width = aw;
  m_atlas_height = ah;

  m_texture_atlas = new Texture{aw, ah};
  
  int aa = 0;
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  m_texture_atlas->Bind();
  for (GLubyte c = 32; c < 128; c++)
  {
    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, aa, 0, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);
    
    character_info ch_info =
    {
        m_face->glyph->advance.x,
        m_face->glyph->advance.y,
        m_face->glyph->bitmap.width,
        m_face->glyph->bitmap.rows,
        m_face->glyph->bitmap_left,
        m_face->glyph->bitmap_top,
        (float)aa / m_atlas_width
    };
    m_char_infos.insert(std::pair<GLchar, character_info>(c, ch_info));
    aa += m_face->glyph->bitmap.width;
  }
  m_texture_atlas->Unbind();

  FT_Done_Face(m_face);
  FT_Done_FreeType(m_ft);
}
TextLayer::~TextLayer() {
    delete m_texture_atlas;
    for (auto c : m_chars)
        delete c.second.Tex;
}

void TextLayer::Render() {
  this->Shad->Enable();
  this->Ren->Begin();

  std::string text = "Nikte!";
  GLfloat scale = 1.f;
  GLfloat x = 32.f;
  GLfloat y = 32.f;
  std::string::const_iterator c;

  for (c = text.begin(); c != text.end(); c++)
  {
      character_info ch = m_char_infos[*c];
      GLfloat xpos = x + ch.bl * scale;
      GLfloat ypos = y - ch.bt * scale;
      GLfloat w = ch.bw * scale;
      GLfloat h = ch.bh * scale;

      Sprite sprite = Sprite{glm::vec3(xpos, ypos, 0.f), glm::vec2(w, h), glm::vec2(ch.tx, 0.f), *m_texture_atlas};
      x += (ch.ax >> 6) * scale;
      this->Ren->Submit(sprite);
  }

  this->Ren->End();
  this->Ren->Draw();
}

} // namespace graphics
} // namespace stella