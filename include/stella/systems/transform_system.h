#pragma once

#include "./system.h"
#include "../components.h"

namespace stella
{
namespace systems
{
class TransformSystem : public System
{
  public:
    TransformSystem() { }

    ~TransformSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::TransformComponent>(entt::get<components::DimensionComponent, components::SpriteComponent>).each([](auto entity, auto &trans, auto &dim, auto &sprite)
      {
        if ((int)(sprite.Sprite->RealDimensions.x) != dim.w) {
          trans.Scale.x *= static_cast<float>(dim.w/sprite.Sprite->Dimensions.x);
          sprite.Sprite->RealDimensions.x = dim.w;
        }
        if ((int)(sprite.Sprite->RealDimensions.y) != dim.h) {
          trans.Scale.y *= static_cast<double>(dim.h/sprite.Sprite->Dimensions.y);
          sprite.Sprite->RealDimensions.y = dim.h;
        }
        sprite.Sprite->SetScale(trans.Scale);
        sprite.Sprite->SetRotation(trans.Rotation);
      });
    }
};
} // namespace systems
} // namespace stella


