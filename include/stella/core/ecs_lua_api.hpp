#pragma once

#include "basic_lua_api.hpp"
#include "stella/perlin.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace script
{
  class ECSLuaApi : public BasicLuaApi
  {
  private:
    entt::registry& m_registry;
    stella::PerlinNoise m_noise_generator;

  public:
    ECSLuaApi (entt::registry& registry);

  private:
    std::tuple<int, int, int> get_position (entt::registry::entity_type entity);
    void create_layer (const sol::table& obj);
    const entt::registry::entity_type create_entity();
    void add_sprite_component (entt::registry::entity_type id, const sol::table& obj);
    void add_position_component (entt::registry::entity_type index, const sol::table& obj);
    void add_dimension_component (entt::registry::entity_type id, const sol::table& obj);
    void add_animation_component (entt::registry::entity_type id, const sol::table& obj);
    void add_tileview_component (entt::registry::entity_type id, const sol::table& obj);
    void add_movement_component (entt::registry::entity_type id, const sol::table& obj);
    void add_player_component (entt::registry::entity_type id, const sol::table& obj);
    void add_body_component (entt::registry::entity_type id, const sol::table& obj);
    void add_text_component (entt::registry::entity_type id, const sol::table& obj);
    void add_particle_emitter_component (entt::registry::entity_type id, const sol::table& obj);
    void add_tile_component (entt::registry::entity_type id, const sol::table& obj);
    void add_scroll_component (entt::registry::entity_type id, const sol::table& obj);
    void add_name_component (entt::registry::entity_type id, const sol::table& obj);
    void add_npc_component (entt::registry::entity_type id, const sol::table& obj);
    void add_character_animation_component (entt::registry::entity_type id);
    void add_component (const sol::table& obj);

    inline double get_perlin_double (double x, double y) { return m_noise_generator.get_double (x, y); }

    inline int get_perlin_int (double min, double max, int pos)
    {
      double x = static_cast<double> (pos % 256);
      double y = static_cast<double> ((pos / 256) % 256);
      return m_noise_generator.get_int (static_cast<int> (min), static_cast<int> (max), x / 100.f, y / 100.f);
    }
    inline int get_random (double min, double max)
    {
      return static_cast<int> (min) + rand() % ((static_cast<int> (max) + 1) - static_cast<int> (min));
    }
  };
} // namespace script
} // namespace stella
