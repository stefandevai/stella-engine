#pragma once

#include "./system.h"
#include "stella/components/color_component.h"
#include "stella/components/sprite_component.h"

namespace stella
{
namespace systems
{
class ColorSystem : public System
{
  public:
    ColorSystem(entt::registry &registry)
    {
      registry.on_construct<components::ColorComponent>().connect<&ColorSystem::initialize_color>(this);
    }

    ~ColorSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
    //   registry.group<components::ColorComponent>(entt::get<components::SpriteComponent>).each([&registry](auto entity, auto &color, auto &sprite)
    //   {
        
    //   });
    }
  private:
    ColorSystem() = delete;
    
    void initialize_color(entt::registry &registry, entt::entity entity, components::ColorComponent &color)
    {
      if (registry.has<components::SpriteComponent>(entity))
      {
          auto sprite = registry.get<components::SpriteComponent>(entity);
          sprite.Sprite->SetDirectColor(color.int_color);
      }
    }
};
} // namespace systems
} // namespace stella

