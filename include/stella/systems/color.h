#pragma once

#include "./system.h"
#include "stella/components/color.h"
#include "stella/components/sprite.h"

namespace stella
{
namespace systems
{
  class Color : public System
  {
  public:
    Color (entt::registry& registry)
    {
      registry.on_construct<components::Color>().connect<&Color::initialize_color> (this);
    }

    ~Color() override {}

    void update (entt::registry& registry, const double dt) override
    {
      //   registry.group<components::Color>(entt::get<components::Sprite>).each([&registry](auto
      //   entity, auto &color, auto &sprite)
      //   {

      //   });
    }

  private:
    Color() = delete;

    void initialize_color (entt::registry& registry, entt::entity entity, components::Color& color)
    {
      if (registry.has<components::Sprite> (entity))
      {
        auto sprite = registry.get<components::Sprite> (entity);
        sprite.sprite->SetDirectColor (color.int_color);
      }
    }
  };
} // namespace systems
} // namespace stella
