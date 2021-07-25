#pragma once

#include "component.hpp"

#include <string>
#include <vector>

#include <entt/entity/registry.hpp> // IWYU pragma: export

namespace stella
{
namespace component
{
  class Text : public Component
  {
    public:
      std::vector<entt::registry::entity_type> char_entities{};

      Text() : Component ("Text") {}
      Text (const std::wstring text, const std::string font_name, const unsigned int font_size, const std::string& color = "#ffffffff", const bool is_static = false)
        : Component ("Text"), m_text (text), m_font_name (font_name), m_color (color), m_font_size (font_size), m_is_static (is_static)
      {
      }
      inline const std::wstring get_text () const { return m_text; };
      inline const std::string get_color () const { return m_color; };
      inline const std::string get_font_name () const { return m_font_name; };
      inline const bool get_is_static () const { return m_is_static; };
      inline const size_t get_font_size () const { return m_font_size; };

      inline void set_text (const std::wstring& text) { m_text = text; };
      inline void set_color (const std::string& color) { m_color = color; };
      inline void set_is_static (const bool is_static) { m_is_static = is_static; };

    private:
      std::wstring m_text{};
      std::string m_font_name{}, m_color{};
      size_t m_font_size = 1;
      /* bool read      = false; // If the text was already read by a NPC */
      bool m_is_static = false;
  };

} // namespace component
} // namespace stella
