#include "stella/core/script_api.hpp"
#include "stella/component_types.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/mesh.hpp"
#include "stella/components/position2.hpp"
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
}

void ScriptAPI::m_init_component_usertypes()
{
  m_lua.new_usertype<component::Sprite>("Sprite",
      sol::constructors<component::Sprite(const std::string&), component::Sprite(const std::string&, const int)>(),
      sol::base_classes, sol::bases<component::Component>(),
      "set_frame", &component::Sprite::set_frame,
      "increment_frame", &component::Sprite::increment_frame,
      "decrement_frame", &component::Sprite::decrement_frame);

  m_lua.new_usertype<component::Mesh>("Mesh",
      sol::constructors<component::Mesh(const std::string&)>(),
      sol::base_classes, sol::bases<component::Component>());

  m_lua.new_usertype<component::Position2>("Position",
      sol::constructors<component::Position2(const float, const float), component::Position2(const float, const float, const float)>(),
      sol::base_classes, sol::bases<component::Component>(),
      "x", &component::Position2::x,
      "y", &component::Position2::y,
      "z", &component::Position2::z);
}

} // namespace stella::core
