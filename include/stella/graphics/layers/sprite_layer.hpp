#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <set>
#include <vector>

#include "stella/graphics/layers/layer.hpp"
#include "stella/graphics/sprite_renderer.hpp"
#include "stella/graphics/shader.hpp"
#include "stella/graphics/sprite.hpp"

namespace stella
{
namespace graphics
{
  class SpriteLayer : public Layer
  {
  private:
    struct CompSpriteZ
    {
      bool operator() (const std::shared_ptr<Sprite>& lhs, const std::shared_ptr<Sprite>& rhs) const noexcept
      {
        // If their z is equal, use y position as a second parameter of differentiation
        if (lhs->Pos.z == rhs->Pos.z)
        {
          return ((lhs->Pos.y + lhs->Dimensions.y) < (rhs->Pos.y + rhs->Dimensions.y));
        }

        return (lhs->Pos.z < rhs->Pos.z);
      }
    };

  protected:
    // std::shared_ptr<Renderer> Ren;
    // std::shared_ptr<Shader> Shad;
    std::vector<std::shared_ptr<Sprite>> Sprites;
    // glm::mat4 ViewMatrix;

    std::multiset<std::shared_ptr<Sprite>, CompSpriteZ> m_sprites;

  public:
    // bool Fixed;

    //virtual ~SpriteLayer();
    virtual void Add (std::shared_ptr<Sprite> sprite);
    virtual void Remove (std::shared_ptr<Sprite> sprite);
    virtual void Render();
    //virtual void SetViewMatrix (glm::mat4 view);

  protected:
    SpriteLayer (std::shared_ptr<SpriteRenderer> renderer, bool fixed = true);
  };
} // namespace graphics
} // namespace stella
