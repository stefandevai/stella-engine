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
    
  for (GLubyte c = 0; c < 128; c++)
  {
    if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
    {
        std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
        continue;
    }

    Texture *tex = new Texture;
    tex->LoadChar(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, m_face->glyph->bitmap.buffer);

    Character character =
    {
        tex->GetID(),
        glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
        glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
        m_face->glyph->advance.x,
        tex
    };
    m_chars.insert(std::pair<GLchar, Character>(c, character));
  }

  FT_Done_Face(m_face);
  FT_Done_FreeType(m_ft);
}
TextLayer::~TextLayer() {
    for (auto c : m_chars)
        delete c.second.Tex;
}

void TextLayer::Render() {
  this->Shad->Enable();
  this->Ren->Begin();

  std::string text = "NIKTE";
  GLfloat scale = 1.f;
  GLfloat x = 32.f;
  GLfloat y = 32.f;
  std::string::const_iterator c;

  for (c = text.begin(); c != text.end(); c++)
  {
      Character ch = m_chars[*c];
      GLfloat xpos = x + ch.Bearing.x * scale;
      GLfloat ypos = y - ch.Bearing.y * scale;
      GLfloat w = ch.Size.x * scale;
      GLfloat h = ch.Size.y * scale;
      Sprite sprite = Sprite{xpos, ypos, w, h, *ch.Tex};
      x += (ch.Advance >> 6) * scale;
      this->Ren->Submit(sprite);
  }

  this->Ren->End();
  this->Ren->Draw();
}

} // namespace graphics
} // namespace stella