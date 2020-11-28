#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // IWYU pragma: export
#include <entt/entity/registry.hpp>

namespace stella
{
enum class ComponentType;

namespace component
{
  struct Component;
}
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
  template <typename T>
  T& api_get_component (const entt::entity entity)
  {
    return m_registry.get<T>(entity);
  }

  template <typename T>
  void api_add_component (const entt::entity entity, T& component)
  {
    m_registry.emplace<T>(entity, component);
  }

  template <typename T>
  void api_remove_component (const entt::entity entity)
  {
    m_registry.remove_if_exists<T>(entity);
  }

private:
  entt::registry& m_registry;
  sol::state m_lua;
  std::string m_filepath;

private:
  void m_init_api();
  void m_init_component_usertypes();
};
} // namespace stella::core
