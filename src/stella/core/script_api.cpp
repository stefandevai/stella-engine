#include "stella/core/script_api.hpp"
#include "stella/component_types.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/mesh.hpp"
#include "stella/components/position2.hpp"
#include "stella/components/animation_player.hpp"
#include <spdlog/spdlog.h>

namespace stella::core
{
ScriptAPI::ScriptAPI (entt::registry& registry) : m_registry (registry)
{
  m_lua.open_libraries (sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
  m_init_api();
  m_init_component_usertypes();
}

void ScriptAPI::load (const std::string& filepath)
{
  auto loading_result = m_lua.load_file(filepath);

  if (!loading_result.valid())
  {
    spdlog::critical ("Could not load script {}:\n{}", filepath, static_cast<sol::error>(loading_result).what());
    return;
  }

  auto execution_result = loading_result();
  if (!execution_result.valid())
  {
    spdlog::critical ("Could not execute script {}:\n{}", filepath, static_cast<sol::error>(execution_result).what());
    return;
  }

  m_filepath = filepath;
}

void ScriptAPI::reload()
{
  if (m_filepath.empty())
  {
    spdlog::warn ("No script filepath selected.");
    return;
  }

  m_lua.load (m_filepath);
}

const entt::entity ScriptAPI::api_create_entity() { return m_registry.create(); }

void ScriptAPI::m_init_api()
{
  m_lua.set_function ("create_entity", &ScriptAPI::api_create_entity, this);

  m_lua.set_function ("add_component_sprite", &ScriptAPI::api_add_component<component::Sprite>, this);
  m_lua.set_function ("get_component_sprite", &ScriptAPI::api_get_component<component::Sprite>, this);
  m_lua.set_function ("remove_component_sprite", &ScriptAPI::api_remove_component<component::Sprite>, this);

  m_lua.set_function ("add_component_position", &ScriptAPI::api_add_component<component::Position2>, this);
  m_lua.set_function ("get_component_position", &ScriptAPI::api_get_component<component::Position2>, this);
  m_lua.set_function ("remove_component_position", &ScriptAPI::api_remove_component<component::Position2>, this);

  m_lua.set_function ("add_component_mesh", &ScriptAPI::api_add_component<component::Mesh>, this);
  m_lua.set_function ("get_component_mesh", &ScriptAPI::api_get_component<component::Mesh>, this);
  m_lua.set_function ("remove_component_mesh", &ScriptAPI::api_remove_component<component::Mesh>, this);

  m_lua.set_function ("add_component_animation", &ScriptAPI::api_add_component<component::AnimationPlayer>, this);
  m_lua.set_function ("get_component_animation", &ScriptAPI::api_get_component<component::AnimationPlayer>, this);
  m_lua.set_function ("remove_component_animation", &ScriptAPI::api_remove_component<component::AnimationPlayer>, this);
}

void ScriptAPI::m_init_component_usertypes()
{
  using namespace component;

  m_lua.new_usertype<Sprite>("Sprite",
      sol::constructors<Sprite(const std::string&), Sprite(const std::string&, const int)>(),
      sol::base_classes, sol::bases<Component>(),
      "set_frame", &Sprite::set_frame,
      "increment_frame", &Sprite::increment_frame,
      "decrement_frame", &Sprite::decrement_frame);

  m_lua.new_usertype<Mesh>("Mesh",
      sol::constructors<Mesh(const std::string&)>(),
      sol::base_classes, sol::bases<Component>());

  m_lua.new_usertype<Position2>("Position",
      sol::constructors<Position2(const float, const float), Position2(const float, const float, const float)>(),
      sol::base_classes, sol::bases<Component>(),
      "x", &Position2::x,
      "y", &Position2::y,
      "z", &Position2::z);

  m_lua.new_usertype<AnimationPlayer>("Animation",
      sol::constructors<AnimationPlayer()>(),
      sol::base_classes, sol::bases<Component>(),
      "current", &AnimationPlayer::current,
      "add", &AnimationPlayer::add);

  m_lua.new_usertype<AnimationData>("AnimationData",
      sol::meta_function::construct, sol::factories(
        [](const sol::table& obj) {return AnimationData{obj[1].get<std::vector<unsigned int>>()};},
        [](const sol::table& obj, const float step) {return AnimationData{obj[1].get<std::vector<unsigned int>>(), step};},
        [](const sol::table& obj, const float step = 0.1f, const bool loop = false) {return AnimationData{obj[1].get<std::vector<unsigned int>>(), step, loop};}));
      //sol::constructors<AnimationData(const std::vector<unsigned int>&),
                        //AnimationData(const std::vector<unsigned int>&, const float),
                        //AnimationData(const std::vector<unsigned int>&, const float, const bool)>());
}

} // namespace stella::core
