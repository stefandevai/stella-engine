#include "stella/core/script_api.hpp"
#include "stella/component_types.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/mesh.hpp"
#include <spdlog/spdlog.h>

namespace stella::core
{
ScriptAPI::ScriptAPI (entt::registry& registry) : m_registry (registry)
{
  m_lua.open_libraries (sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
  m_lua.set_function ("create_entity", &ScriptAPI::api_create_entity, this);
  m_lua.set_function ("get_component", &ScriptAPI::api_get_component, this);
  m_lua.set_function ("add_component", &ScriptAPI::api_add_component, this);
  m_lua.set_function ("patch_component", &ScriptAPI::api_patch_component, this);
  m_lua.set_function ("remove_component", &ScriptAPI::api_remove_component, this);
}

const entt::entity ScriptAPI::api_create_entity() { return m_registry.create(); }

sol::table ScriptAPI::api_get_component (const entt::entity entity, const sol::table& object)
{
  const auto component_type = m_get_component_type (object);
  sol::table component_table;

  switch (component_type)
  {
    case ComponentType::SPRITE:
      break;
    case ComponentType::MESH:
      break;
    default:
      break;
  }

  return component_table;
}

void ScriptAPI::api_add_component (const entt::entity entity, const sol::table& object)
{
  const auto component_type = m_get_component_type (object);

  switch (component_type)
  {
    case ComponentType::SPRITE:
      break;
    case ComponentType::MESH:
      break;
    default:
      break;
  }
}

void ScriptAPI::api_patch_component (const entt::entity entity, const sol::table& object)
{
  const auto component_type = m_get_component_type (object);
  switch (component_type)
  {
    case ComponentType::SPRITE:
      break;
    case ComponentType::MESH:
      break;
    default:
      break;
  }
}

void ScriptAPI::api_remove_component (const entt::entity entity, const sol::table& object)
{
  const auto component_type = m_get_component_type (object);
  switch (component_type)
  {
    case ComponentType::SPRITE:
      m_registry.remove<component::Sprite> (entity);
      break;

    case ComponentType::MESH:
      m_registry.remove<component::Mesh> (entity);
      break;

    default:
      break;
  }
}

const ComponentType ScriptAPI::m_get_component_type (const sol::table& object)
{
  if (object["component"] == sol::lua_nil)
  {
    spdlog::critical ("No component was given.");
    return ComponentType::NONE;
  }

  const std::string& component_tag = object["component"];
  ComponentType type;

  try
  {
    type = COMPONENT_MAP.at (component_tag);
  }
  catch (std::out_of_range& e)
  {
    spdlog::critical ("Unknow component type: {}\n{}", component_tag, e.what());
    return ComponentType::NONE;
  }

  return type;
}

} // namespace stella::core
