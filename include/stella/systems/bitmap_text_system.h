#pragma once

#include "../components/bitmap_text_component.h"
#include "../components/dimension_component.h"
#include "../components/position_component.h"
#include "../components/sprite_component.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class BitmapTextSystem : public System
  {
  public:
    BitmapTextSystem (entt::registry& registry)
    {
      registry.on_construct<components::BitmapTextComponent>().connect<&BitmapTextSystem::initialize_text> (this);
    }

    ~BitmapTextSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::BitmapTextComponent> (
              entt::get<components::PositionComponent, components::DimensionComponent>)
          .each ([&registry] (auto entity, auto& text, auto& pos, auto& dim) {
            if (!text.IsStatic)
            {
              int stride       = 0;
              auto char_entity = text.char_entities.begin();
              auto c           = text.Text.begin();
              text.Spaces      = 0;

              for (; c != text.Text.end(); ++c)
              {
                if (char_entity == text.char_entities.end())
                  break;
                int frame = (int) *c - 33;
                if (frame == -1)
                {
                  stride += 1;
                  text.Spaces += 1;
                }
                else
                {
                  auto& char_pos = registry.get<components::PositionComponent> (*char_entity);
                  auto& char_spr = registry.get<components::SpriteComponent> (*char_entity);
                  char_pos.x     = pos.x + dim.w * stride;
                  char_spr.Sprite->SetDirectFrame (frame);
                  ++char_entity;
                  ++stride;
                }
              }

              // If the text is bigger than the last printed
              if (c != text.Text.end())
              {
                for (; c != text.Text.end(); ++c)
                {
                  int frame = (int) *c - 33;
                  if (frame == -1)
                  {
                    ++stride;
                    ++text.Spaces;
                  }
                  else
                  {
                    auto new_char_entity = registry.create();
                    registry.assign<components::SpriteComponent> (
                        new_char_entity, text.Name, glm::vec2 (dim.w, dim.h), "text", frame);
                    registry.assign<components::PositionComponent> (new_char_entity, pos.x + dim.w * stride, pos.y);
                    registry.assign<components::DimensionComponent> (new_char_entity, dim.w, dim.h);
                    text.char_entities.push_back (new_char_entity);
                    ++stride;
                  }
                }
              }

              // If the text is smaller than the last printed
              else if (char_entity != text.char_entities.end())
              {
                auto iterator_copy = char_entity;
                for (; char_entity != text.char_entities.end(); ++char_entity)
                  registry.destroy (*char_entity);

                text.char_entities.erase (iterator_copy, text.char_entities.end());
              }
            }
          });
    }

  private:
    BitmapTextSystem() = delete;

    void initialize_text (entt::registry& registry, entt::entity entity, components::BitmapTextComponent& text)
    {
      text.Spaces = 0;
      int stride  = 0;
      auto pos    = registry.get<components::PositionComponent> (entity);
      auto dim    = registry.get<components::DimensionComponent> (entity);

      for (auto c : text.Text)
      {
        int frame = (int) c - 33;
        if (frame == -1)
        {
          ++stride;
          ++text.Spaces;
        }
        else
        {
          auto char_entity = registry.create();
          registry.assign<components::SpriteComponent> (
              char_entity, text.Name, glm::vec2 (dim.w, dim.h), "text", frame);
          registry.assign<components::PositionComponent> (char_entity, pos.x + dim.w * stride, pos.y);
          registry.assign<components::DimensionComponent> (char_entity, dim.w, dim.h);
          text.char_entities.push_back (char_entity);
          ++stride;
        }
      }
    }
  };
} // namespace systems
} // namespace stella
