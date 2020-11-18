#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1

#include <string>
#include <vector>
#include <memory>
#include <entt/entity/registry.hpp>
#include <sol/sol.hpp> // IWYU pragma: export
#include "stella/graphics/display.hpp"
#include "stella/graphics/font.hpp"
#include "stella/types.hpp"
#include "stella/core/scene.hpp"
#include "stella/core/json.hpp"

namespace stella
{
  class Game
  {
  public:
    Game (const std::string& script_path);
    void run();
    inline const uint_fast32_t get_width() const { return m_display.get_width(); };
    inline const uint_fast32_t get_height() const { return m_display.get_height(); };

    void add_scene(std::shared_ptr<core::Scene>& scene);
    void create_scene(const std::string& name, const std::string& script_path);
    void load_scene(const std::string& name);
    void update (const double dt);
    void render (const double dt);

  private:
    sol::state m_lua;
    core::JSON m_json{m_script_path};
    entt::registry m_registry;
    unsigned m_initial_width = 0;
    unsigned m_initial_height = 0;
    std::string m_initial_title;
    std::string m_script_path;
    graphics::Display m_display{m_initial_width, m_initial_height, m_initial_title};
    TextureManager m_textures;

    // TODO: Use another data structure to hold scenes and names
    std::vector<std::shared_ptr<core::Scene>> m_scenes;
    std::shared_ptr<core::Scene> m_current_scene = nullptr;
    
#ifdef STELLA_BUILD_EDITOR
    friend class editor::Editor;
#endif

  private:
    void m_init_variables();
    void m_init_scenes();
  };
}
