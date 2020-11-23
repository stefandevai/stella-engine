#include "stella/systems/text.hpp"
#include "stella/components/charcode.hpp"
#include "stella/components/color.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/position.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/typewriter.hpp"
#include "stella/components/timer.hpp"

namespace stella
{
namespace system
{
  Text::Text (entt::registry& registry, FontManager& fonts)
    : System ("text"), m_fonts (fonts)
  {
    registry.on_construct<component::Text>().connect<&Text::initialize_text> (this);
    registry.on_destroy<component::Text>().connect<&Text::delete_text> (this);
  }

  void Text::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Text> (entt::get<component::Position>)
        .each ([&registry, this, dt] (auto entity, auto& text, auto& pos) {
          if (!text.is_static)
          {
            auto font = m_fonts.load (text.font_name);
            // Define a char's x position in relation to the chars before it
            float acc_char_posx = pos.x;

            for (const auto chr : text.char_entities)
            {
              auto& chr_pos        = registry.get<component::Position> (chr);
              const auto& chr_code = registry.get<component::Charcode> (chr);
              const auto& chr_data = font->get_char_data (chr_code.code);

              chr_pos.x = acc_char_posx + chr_data.bl;
              chr_pos.y = pos.y - chr_data.bt;
              acc_char_posx += (chr_data.ax >> 6);
            }

            if (registry.has<component::Typewriter> (entity))
            {
              m_typewrite (registry, entity, dt);
            }
          }
        });
  }

  void Text::m_typewrite (entt::registry& registry, entt::entity entity, const double dt)
  {
    auto& typewriter = registry.get<component::Typewriter> (entity);

    // If we haven't finished to display the entire text
    if (typewriter.char_index < typewriter.target_text.size())
    {
      if (typewriter.time_accumulator >= typewriter.type_delay)
      {
        typewriter.time_accumulator = 0.0;
        // std::wcout << typewriter.target_text.at(typewriter.char_index) << '\n';
        m_append_to_text (registry, entity, typewriter.target_text.at (typewriter.char_index));
        ++typewriter.char_index;
      }
      else
      {
        typewriter.time_accumulator += dt * 1000.0;
      }
    }
    // We finished displaying the entire text
    else
    {
      if (registry.has<component::Timer> (entity) && !typewriter.has_finished)
      {
        // Activate the timer after all the text is displayed
        auto& timer             = registry.get<component::Timer> (entity);
        timer.active            = true;
        typewriter.has_finished = true;
      }
    }
  }

  void Text::m_append_to_text (entt::registry& registry, entt::entity entity, const wchar_t chr)
  {
    auto& text      = registry.get<component::Text> (entity);
    auto& pos       = registry.get<component::Position> (entity);
    auto& dim       = registry.get<component::Dimension> (entity);
    auto font       = m_fonts.load (text.font_name);
    float char_maxh = 0.f;

    const auto& ch     = font->get_char_data (chr);
    auto char_posx     = static_cast<float> (pos.x) + dim.w;
    const GLfloat xpos = char_posx + ch.bl * text.scale;
    const GLfloat ypos = pos.y - ch.bt * text.scale;
    const GLfloat w    = ch.bw * text.scale;
    const GLfloat h    = ch.bh * text.scale;
    dim.w += (ch.ax >> 6) * text.scale;

    auto char_entity = registry.create();

    // If the character has a graphical representation
    if (w > 0.f && h > 0.f)
    {
      registry.emplace<component::Charcode> (char_entity, chr);
      auto& sprite          = registry.emplace<component::SpriteT> (char_entity, text.font_name);
      sprite.texture_ptr    = font->get_atlas();
      sprite.layer          = "text";
      sprite.top_left.x     = ch.tx;
      sprite.bottom_right.x = w + ch.tx;
      sprite.bottom_right.y = h;
      sprite.set_uv (glm::vec2 (ch.tx, 1.0f));

      registry.emplace<component::Position> (char_entity, xpos, ypos, 30);
      registry.emplace<component::Dimension> (char_entity, w, h);
      registry.emplace<component::Color> (char_entity, text.color);
    }
    // Else, the character is an space
    else
    {
      registry.emplace<component::Charcode> (char_entity, chr);
      registry.emplace<component::Position> (char_entity, xpos, ypos);
    }
    text.char_entities.push_back (char_entity);

    char_posx += (ch.ax >> 6) * text.scale;
    char_maxh = std::max (char_maxh, h);
  }

  void Text::initialize_text (entt::registry& registry, entt::entity entity)
  {
    auto& text      = registry.get<component::Text> (entity);
    auto font       = m_fonts.load (text.font_name);
    const auto& pos = registry.get<component::Position> (entity);
    float char_posx = (float) pos.x;
    float char_maxh = 0.f;

    double max_text_width = 0.f;
    // double max_text_height = 0.f;

    // TODO: If there are defined dimensions, the text should remain within
    // its constraints
    if (registry.has<component::Dimension> (entity))
    {
      const auto& dim = registry.get<component::Dimension> (entity);
      max_text_width  = dim.w;
      // max_text_height = dim.h;
    }

    // Add only one character if it has the typewriter effect
    if (registry.has<component::Typewriter> (entity))
    {
      auto& typewriter       = registry.get<component::Typewriter> (entity);
      typewriter.target_text = text.text;

      if (!text.text.empty())
      {
        text.text = text.text.substr (0, 1);
      }
      ++typewriter.char_index;
    }

    for (wchar_t c : text.text)
    {
      const auto& ch     = font->get_char_data (c);
      const GLfloat xpos = char_posx + ch.bl * text.scale;
      const GLfloat ypos = pos.y - ch.bt * text.scale;
      const GLfloat w    = ch.bw * text.scale;
      const GLfloat h    = ch.bh * text.scale;

      auto char_entity = registry.create();

      // If the character has a graphical representation
      if (w > 0.f && h > 0.f)
      {
        registry.emplace<component::Charcode> (char_entity, c);

        // auto& sprite = registry.emplace<component::SpriteT> (char_entity, text.font_name);
        // sprite.texture_ptr = font->get_atlas();
        // sprite.layer = "text";
        // sprite.top_left.x = ch.tx;
        // sprite.bottom_right.x = w + ch.tx;
        // sprite.bottom_right.y = h;
        // sprite.set_uv(glm::vec2(ch.tx, 1.0f));
        auto& sprite          = registry.emplace<component::SpriteT> (char_entity, text.font_name);
        sprite.texture_ptr    = font->get_atlas();
        sprite.layer          = "text";
        sprite.top_left.x     = ch.tx;
        sprite.bottom_right.x = w + ch.tx;
        sprite.bottom_right.y = h;
        sprite.set_uv (glm::vec2 (ch.tx, 1.0f));

        registry.emplace<component::Position> (char_entity, xpos, ypos, 30);
        registry.emplace<component::Dimension> (char_entity, w, h);
        registry.emplace<component::Color> (char_entity, text.color);
      }
      // Else, the character is an space
      else
      {
        registry.emplace<component::Charcode> (char_entity, c);
        registry.emplace<component::Position> (char_entity, xpos, ypos);
      }
      text.char_entities.push_back (char_entity);

      char_posx += (ch.ax >> 6) * text.scale;
      char_maxh = std::max (char_maxh, h);
    }

    // If there are no defined dimensions, the text will be in one line
    if (max_text_width == 0.f)
    {
      auto& dim = registry.get_or_emplace<component::Dimension, float, float> (entity, 0.f, 0.f);
      dim.w     = char_posx - pos.x;
      dim.h     = char_maxh;
    }
  }

  void Text::delete_text (entt::registry& registry, entt::entity entity)
  {
    auto& text = registry.get<component::Text> (entity);
    for (auto& ch : text.char_entities)
    {
      registry.destroy (ch);
    }
  }
} // namespace system
} // namespace stella
