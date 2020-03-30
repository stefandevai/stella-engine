#pragma once

#include "../components.hpp"
#include "./system.hpp"

namespace stella
{
namespace system
{
  class Transform : public System
  {
  public:
    Transform() {}

    ~Transform() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<component::Transform> (entt::get<component::Dimension, component::Sprite>)
          .each ([] (auto entity, auto& trans, auto& dim, auto& sprite) {
            if (sprite.sprite)
            {
              if (static_cast<int> (sprite.sprite->RealDimensions.x) != dim.w)
              {
                trans.Scale.x *= static_cast<float> (dim.w / sprite.sprite->Dimensions.x);
                sprite.sprite->RealDimensions.x = dim.w;
              }
              if (static_cast<int> (sprite.sprite->RealDimensions.y) != dim.h)
              {
                trans.Scale.y *= static_cast<double> (dim.h / sprite.sprite->Dimensions.y);
                sprite.sprite->RealDimensions.y = dim.h;
              }
              sprite.sprite->SetScale (trans.Scale);
              sprite.sprite->SetRotation (trans.Rotation);
            }
          });
    }
  };
} // namespace system
} // namespace stella
