#include "stella/graphics/layers/sprite_layer.hpp"
#include "stella/graphics/opengl.hpp" // IWYU pragma: export

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp> // IWYU pragma: export

namespace stella
{
namespace graphics
{
  SpriteLayer::SpriteLayer (std::shared_ptr<Renderer> renderer, bool fixed) : Layer (renderer, fixed) {}

  void SpriteLayer::Add (std::shared_ptr<Renderable> renderable)
  {
    auto spr = std::dynamic_pointer_cast<Sprite> (renderable);
    if (spr != nullptr)
    {
      Add (spr);
    }
  }

  void SpriteLayer::Remove (std::shared_ptr<Renderable> renderable)
  {
    auto spr = std::dynamic_pointer_cast<Sprite> (renderable);
    if (spr != nullptr)
    {
      Remove (spr);
    }
  }

  void SpriteLayer::Add (std::shared_ptr<Sprite> sprite) { m_sprites.insert (sprite); }

  void SpriteLayer::Remove (std::shared_ptr<Sprite> sprite)
  {
    auto it = std::find (m_sprites.begin(), m_sprites.end(), sprite);
    m_sprites.erase (it);
  }

  void SpriteLayer::Render()
  {
    this->Shad->Enable();
    if (!this->Fixed)
    {
      this->Shad->SetMat4 ("view", this->ViewMatrix);
    }
    this->Ren->Begin();

    // glm::mat4 trans;
    // trans = glm::translate(trans, glm::vec3(360.0f, 202.0f, 0.0f));
    // trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 1.0f));
    // trans = glm::rotate(trans, glm::radians(-5.0f), glm::vec3(0.0f,
    // 0.0f, 1.0f)); trans = glm::translate(trans, glm::vec3(-360.0f, -202.0f,
    // 0.0f)); this->Ren->PushTransformation(trans);

    for (auto i : m_sprites)
      this->Ren->Submit (i);

    // this->Ren->PopTransformation();

    this->Ren->End();
    this->Ren->Draw();
    // this->Shad->Disable();
  }

  // void SpriteLayer::SetViewMatrix (glm::mat4 view) { this->ViewMatrix = view; }
} // namespace graphics
} // namespace stella
