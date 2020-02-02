#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <set>
#include <vector>

#include "stella/graphics/renderer.h"
#include "stella/graphics/shader.h"
#include "stella/graphics/sprite.h"

namespace stella
{
namespace graphics
{
  class Layer
  {
  private:
    struct CompSpriteZ
    {
      bool operator() (const std::shared_ptr<Sprite>& lhs, const std::shared_ptr<Sprite>& rhs) const noexcept
      {
        return ((lhs->Pos.y + lhs->Dimensions.y) < (rhs->Pos.y + rhs->Dimensions.y));
      }
    };

  protected:
    std::shared_ptr<Renderer> Ren;
    std::shared_ptr<Shader> Shad;
    std::vector<std::shared_ptr<Sprite>> Sprites;
    glm::mat4 ViewMatrix;

    std::multiset<std::shared_ptr<Sprite>, CompSpriteZ> m_sprites;

  public:
    bool Fixed;

    virtual ~Layer();
    virtual void Add (std::shared_ptr<Sprite> sprite);
    virtual void Remove (std::shared_ptr<Sprite> sprite);
    virtual void Render();
    virtual void SetViewMatrix (glm::mat4 view);

  protected:
    Layer (std::shared_ptr<Renderer> renderer, bool fixed = true);
  };
} // namespace graphics
} // namespace stella
