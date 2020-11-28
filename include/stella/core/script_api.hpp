#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // IWYU pragma: export
#include <entt/entity/registry.hpp>

namespace stella
{
enum class ComponentType;
}

namespace stella::core
{
class ScriptAPI
{
public:
  ScriptAPI (entt::registry& registry);

  void load (const std::string& filepath);
  void reload();
  inline const std::string& get_filepath() const { return m_filepath; }
  // API
  const entt::entity api_create_entity();
  sol::table api_get_component (const entt::entity entity, const sol::table& object);
  void api_add_component (const entt::entity entity, const sol::table& object);
  void api_patch_component (const entt::entity entity, const sol::table& object);
  void api_remove_component (const entt::entity entity, const sol::table& object);

private:
  entt::registry& m_registry;
  sol::state m_lua;
  std::string m_filepath;

private:
  const ComponentType m_get_component_type (const sol::table& object);
};
} // namespace stella::core
