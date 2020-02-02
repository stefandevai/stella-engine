#pragma once

#include "../components/bitmap_text.h"
#include "../components/dimension.h"
#include "../components/position.h"
#include "../components/sprite.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class BitmapText : public System
  {
  public:
    BitmapText (entt::registry& registry)
    {
      registry.on_construct<components::BitmapText>().connect<&BitmapText::initialize_text> (this);
    }

    ~BitmapText() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry
          .group<components::BitmapText> (
              entt::get<components::Position, components::Dimension>)
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
                  auto& char_pos = registry.get<components::Position> (*char_entity);
                  auto& char_spr = registry.get<components::Sprite> (*char_entity);
                  char_pos.x     = pos.x + dim.w * stride;
                  char_spr.sprite->SetDirectFrame (frame);
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
                    registry.assign<components::Sprite> (
                        new_char_entity, text.Name, glm::vec2 (dim.w, dim.h), "text", frame);
                    registry.assign<components::Position> (new_char_entity, pos.x + dim.w * stride, pos.y);
                    registry.assign<components::Dimension> (new_char_entity, dim.w, dim.h);
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
    BitmapText() = delete;

    void initialize_text (entt::registry& registry, entt::entity entity, components::BitmapText& text)
    {
      text.Spaces = 0;
      int stride  = 0;
      auto pos    = registry.get<components::Position> (entity);
      auto dim    = registry.get<components::Dimension> (entity);

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
          registry.assign<components::Sprite> (
              char_entity, text.Name, glm::vec2 (dim.w, dim.h), "text", frame);
          registry.assign<components::Position> (char_entity, pos.x + dim.w * stride, pos.y);
          registry.assign<components::Dimension> (char_entity, dim.w, dim.h);
          text.char_entities.push_back (char_entity);
          ++stride;
        }
      }
    }
  };
} // namespace systems
} // namespace stella
