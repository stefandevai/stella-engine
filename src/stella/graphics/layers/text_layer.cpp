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
}

TextLayer::~TextLayer() {}

// void TextLayer::Render() {
//   this->Shad->Enable();
//   this->Ren->Begin();

//   std::string text = "Zahra Ouahbi, je t'aime beaucoup!";
//   GLfloat scale = 1.f;
//   GLfloat x = 32.f;
//   GLfloat y = 32.f;
//   std::string::const_iterator c;

//   for (c = text.begin(); c != text.end(); c++)
//   {
//       CharacterData ch = m_font.get_char_data(*c);
      
//       GLfloat xpos = x + ch.bl * scale;
//       GLfloat ypos = y - ch.bt * scale;
//       GLfloat w = ch.bw * scale;
//       GLfloat h = ch.bh * scale;

//       if (w > 0.f && h > 0.f)
//       {
//         Sprite sprite = Sprite{glm::vec3(xpos, ypos, 0.f), glm::vec2(w, h), glm::vec2(ch.tx, 0.f), *m_font.get_atlas()};
//         this->Ren->Submit(sprite);
//       }
      
//       x += (ch.ax >> 6) * scale;
//   }

//   this->Ren->End();
//   this->Ren->Draw();
// }

} // namespace graphics
} // namespace stella