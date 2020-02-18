#pragma once

#include "stella/graphics/font.h"
#include "stella/systems/system.h"
#include "stella/components/text.h"
#include <string>
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  class Text : public System
  {
  private:
    core::ResourceManager<graphics::Font, const std::string, unsigned>& m_fonts;

  public:
    Text (entt::registry& registry, core::ResourceManager<graphics::Font, const std::string, unsigned>& fonts);
    void update (entt::registry& registry, const double dt) override;

  private:
    Text() = delete;
    void initialize_text (entt::registry& registry, entt::entity entity, component::Text& text);
    void delete_text (entt::registry& registry, entt::entity entity);
    void m_typewrite (entt::registry& registry, entt::entity entity, const double dt);
    void m_append_to_text (entt::registry& registry, entt::entity entity, const wchar_t chr);
  };
} // namespace system
} // namespace stella
