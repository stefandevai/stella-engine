#pragma once

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include "./application.h"
#include "../systems/system.h"
#include "../systems/render_system.h"
#include "../systems/animation_system.h"
#include "./resource.h"
#include "../graphics/texture.h"
#include "../scripting/script_api.h"

namespace stella
{
namespace core
{

  class Game : public Application
  {
    protected:
      entt::registry m_registry;
      script::ScriptApi m_script_api{m_registry};
      audio::SoundPlayer m_sound_player;
      ResourceManager<graphics::Texture> m_textures;
      entt::registry::entity_type m_camera = m_registry.create();
      std::vector<std::shared_ptr<systems::System>> m_systems{
        std::make_shared<systems::RenderSystem>(m_registry, m_textures, m_display),
        std::make_shared<systems::AnimationSystem>()
      };

    public:
      Game(const unsigned width, const unsigned height, const std::string &title);
      virtual ~Game();

    protected:
      void load() override;
      void create_camera(const double x, const double y, const double z);
      void update_camera(const double x, const double y, const double z);
      void load_texture(const std::string &name, const std::string &path);
      void load_font(const std::string &name, const std::string &path);
      void update_systems(const double dt);

      //template <typename T>
      //void add_system(std::shared_ptr<T> game_system)
      //{
        //m_systems.push_back(game_system);
      //}

      template <typename T, typename ... Params>
      void add_system(Params &... params)
      {
        m_systems.push_back(std::make_shared<T>(params...));
      }

  };

} // namespace script
} // namespace st

