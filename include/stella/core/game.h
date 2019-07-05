#pragma once

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include "./application.h"
#include "../systems/system.h"
#include "../systems/render_system.h"
#include "../systems/animation_system.h"

namespace stella
{
namespace core
{

  class Game : public Application
  {
    protected:
      entt::registry m_registry;
      audio::SoundPlayer m_sound_player;
      std::unordered_map<std::string, graphics::Texture*> m_textures;
      std::unordered_map<std::string, graphics::Texture*> m_fonts;
      entt::registry::entity_type m_camera = m_registry.create();
      std::vector<systems::System> m_systems{
        systems::RenderSystem{m_registry, m_textures, m_display},
        systems::AnimationSystem{}
      };

    public:
      Game(const unsigned width, const unsigned height, const std::string &title);
      virtual ~Game();

    protected:
      void load() override;
      void update(const double dt) override;
      void update_systems(const double dt);
      void add_system(systems::System& game_system);
      void load_texture(std::string texture_id, const char *texture_path);
      void load_font(std::string font_id, const char *font_path);
  };

} // namespace script
} // namespace st

