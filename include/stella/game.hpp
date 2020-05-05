#pragma once

#include "./application.hpp"
//#include "stella/audio/soundplayer.hpp"
#include "stella/core/ecs_lua_api.hpp"
#include "stella/core/resource.hpp"
#include "stella/graphics/font.hpp"
#include "stella/graphics/texture.hpp"
#include "stella/physics2d/world.hpp"
//#include "stella/systems/animation.hpp"
#include "stella/systems/color.hpp"
#include "stella/systems/render.hpp"
#include "stella/systems/system.hpp"
#include "stella/systems/timer.hpp"
#include <memory>
#include <string>
#include <vector>

#include "stella/types.hpp"

// #ifdef STELLA_BUILD_EDITOR
// #include "editor/editor_gui.hpp"
// #endif

namespace stella
{
namespace core
{
  class Game : public Application
  {
  protected:
    script::ECSLuaApi m_script_api{m_registry};
    // audio::SoundPlayer m_sound_player;
    TextureManager m_textures;
    FontManager m_fonts;
    entt::registry::entity_type m_camera = m_registry.create();
    std::vector<std::shared_ptr<system::System>> m_systems;
    // std::vector<std::shared_ptr<system::System>> m_systems{
    //     std::make_shared<system::Render> (m_registry, m_textures, m_display),
    //     std::make_shared<system::Color> (m_registry),
    //     std::make_shared<system::Timer> (m_registry),
    //     std::make_shared<system::Animation> (m_registry)};

#ifdef STELLA_BUILD_EDITOR
    // editor::EditorGui m_editor{m_registry};
#endif

  public:
    Game (const unsigned width, const unsigned height, const std::string& title);
    virtual ~Game();
    std::vector<float> get_camera_pos();

  protected:
    void load() override;
    void create_camera (const double x, const double y, const double z);
    void update_camera (const double x, const double y, const double z);
    float get_camera_z();
    void load_texture (const std::string& name, const std::string& path, const unsigned hframes = 1, const unsigned vframes = 1);
    void load_font (const std::string& name, const std::string& path, const unsigned size);
    void update_systems (const double dt);

    template<typename T, typename... Params>
    void add_system (Params&... params)
    {
      m_systems.push_back (std::make_shared<T> (params...));
    }
    std::shared_ptr<system::RenderT> m_render_system;
  };

} // namespace core
} // namespace stella
