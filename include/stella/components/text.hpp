#pragma once

#include <string>
#include <vector>

#include <entt/entity/registry.hpp>

namespace stella
{
namespace component
{
  struct Text
  {
    Text (const std::wstring text,
          const std::string font_name,
          const std::string color = "#ffffffff",
          const bool is_static    = false)
      : text (text), font_name (font_name), color (color), is_static (is_static)
    {
    }
    std::wstring text;
    std::string font_name, color;
    std::vector<entt::registry::entity_type> char_entities;
    double scale = 1.f;
    bool read    = false; // If the text was already read by a NPC
    bool is_static;
  };

} // namespace component
} // namespace stella
