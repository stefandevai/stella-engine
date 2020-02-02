#pragma once

#include "stella/components/charcode.h"
#include "stella/components/color.h"
#include "stella/components/dimension.h"
#include "stella/components/position.h"
#include "stella/components/sprite_component.h"
#include "stella/components/text_component.h"
#include "stella/graphics/font.h"
#include "stella/systems/system.h"

namespace stella
{
namespace systems
{
  class TextSystem : public System
  {
  private:
    // std::map<std::string, std::shared_ptr<graphics::Font>> m_fonts;
    core::ResourceManager<graphics::Font, const std::string, unsigned>& m_fonts;

  public:
    TextSystem (entt::registry& registry, core::ResourceManager<graphics::Font, const std::string, unsigned>& fonts)
      : m_fonts (fonts)
    {
      // m_fonts.insert(std::pair<std::string,
      // std::shared_ptr<graphics::Font>>("1980",
      // std::make_shared<graphics::Font>("assets/fonts/1980.ttf")));
      registry.on_construct<components::TextComponent>().connect<&TextSystem::initialize_text> (this);
      registry.on_destroy<components::TextComponent>().connect<&TextSystem::delete_text> (this);
    }

    ~TextSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<components::TextComponent> (entt::get<components::Position>)
          .each ([&registry, this] (auto entity, auto& text, auto& pos) {
            if (!text.IsStatic)
            {
              auto font = m_fonts.load (text.FontName);
              // Define a char's x position in relation to the chars before it
              float acc_char_posx = pos.x;

              for (const auto chr : text.char_entities)
              {
                auto& chr_pos        = registry.get<components::Position> (chr);
                const auto& chr_code = registry.get<components::Charcode> (chr);
                const auto& chr_data = font->get_char_data (chr_code.code);

                chr_pos.x = acc_char_posx + chr_data.bl;
                chr_pos.y = pos.y - chr_data.bt;
                acc_char_posx += (chr_data.ax >> 6);
              }
            }
          });
    }

  private:
    TextSystem() = delete;

    void initialize_text (entt::registry& registry, entt::entity entity, components::TextComponent& text)
    {
      auto font       = m_fonts.load (text.FontName);
      const auto& pos = registry.get<components::Position> (entity);
      float char_posx = (float) pos.x;
      float char_maxh = 0.f;

      double max_text_width = 0.f;
      // double max_text_height = 0.f;

      // TODO: If there are defined dimensions, the text should remain within
      // its constraints
      if (registry.has<components::Dimension> (entity))
      {
        const auto& dim = registry.get<components::Dimension> (entity);
        max_text_width  = dim.w;
        // max_text_height = dim.h;
      }

      for (wchar_t c : text.Text)
      {
        const auto& ch     = font->get_char_data (c);
        const GLfloat xpos = char_posx + ch.bl * text.scale;
        const GLfloat ypos = pos.y - ch.bt * text.scale;
        const GLfloat w    = ch.bw * text.scale;
        const GLfloat h    = ch.bh * text.scale;

        auto char_entity = registry.create();

        // If the character is an space
        if (w > 0.f && h > 0.f)
        {
          registry.assign<components::Charcode> (char_entity, c);
          registry.assign<components::SpriteComponent> (char_entity,
                                                        glm::vec3 (xpos, ypos, 0.f),
                                                        glm::vec2 (w, h),
                                                        glm::vec2 (ch.tx, 0.f),
                                                        *font->get_atlas(),
                                                        "text");
          registry.assign<components::Position> (char_entity, xpos, ypos);
          registry.assign<components::Dimension> (char_entity, w, h);
          registry.assign<components::Color> (char_entity, text.color);
        }
        // Else, the character has a graphical representation
        else
        {
          registry.assign<components::Charcode> (char_entity, c);
          registry.assign<components::Position> (char_entity, xpos, ypos);
        }
        text.char_entities.push_back (char_entity);

        char_posx += (ch.ax >> 6) * text.scale;
        char_maxh = std::max (char_maxh, h);
      }

      // If there are no defined dimensions, the text will be in one line
      if (max_text_width == 0.f)
      {
        auto& dim = registry.get_or_assign<components::Dimension, float, float> (entity, 0.f, 0.f);
        dim.w     = char_posx - pos.x;
        dim.h     = char_maxh;
      }
    }

    void delete_text (entt::registry& registry, entt::entity entity)
    {
      auto& text = registry.get<components::TextComponent> (entity);
      // std::cout << "here\n";
      for (auto& ch : text.char_entities)
      {
        registry.destroy (ch);
      }
    }
  };
} // namespace systems
} // namespace stella
