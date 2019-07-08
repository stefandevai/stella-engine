#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include <entt/entity/registry.hpp>
#include "stella/perlin.h"

namespace stella
{
namespace script
{
  class LuaInterface
  {
    private:
      sol::state m_lua;
      entt::registry &m_registry;
      stella::PerlinNoise m_noise_generator;

    public:
      LuaInterface(entt::registry &registry);
      ~LuaInterface();

      void run_script(const std::string &script_path);
      void run_function(const std::string &function_name);

      template <typename ... Params>
      inline void run_function(const std::string &function_name, const Params &... params)
      {
        m_lua[function_name](params...);
      }

      template <typename T>
      inline void set_function(const std::string &function_name, T a_function)
      {
        m_lua.set_function(function_name, a_function);
      }

      template <typename T, typename R>
      inline void set_function(const std::string &function_name, T a_function, R a_instance)
      {
        m_lua.set_function(function_name, a_function, a_instance);
      }

      template<typename T>
      inline void set_variable(const std::string &name, T value)
      {
        this->m_lua[name] = value;
      }

      template<typename T>
      inline T get_variable(const std::string &name)
      {
        return this->m_lua.get<T>(name);
      }

    private:
      std::tuple<int,int,int> get_position(entt::registry::entity_type entity);
      void create_layer(const sol::table &obj);
      const entt::registry::entity_type create_entity();
      void add_sprite_component(entt::registry::entity_type id, const sol::table &obj);
      void add_position_component(entt::registry::entity_type index, const sol::table &obj);
      void add_dimension_component(entt::registry::entity_type id, const sol::table &obj);
      void add_animation_component(entt::registry::entity_type id, const sol::table &obj);
      void add_tileview_component(entt::registry::entity_type id, const sol::table &obj);
      void add_movement_component(entt::registry::entity_type id, const sol::table &obj);
      void add_player_component(entt::registry::entity_type id, const sol::table &obj);
      void add_body_component(entt::registry::entity_type id, const sol::table &obj);
      void add_text_component(entt::registry::entity_type id, const sol::table &obj);
      void add_particle_emitter_component(entt::registry::entity_type id, const sol::table &obj);
      void add_tile_component(entt::registry::entity_type id, const sol::table &obj);
      void add_scroll_component(entt::registry::entity_type id, const sol::table &obj);
      void add_component(const sol::table &obj);

      inline double get_perlin_double(double x, double y)
      {
        return m_noise_generator.get_double(x, y);
      }

      inline int get_perlin_int(double min, double max, int pos)
      {
        double x = static_cast<double>(pos % 256);
        double y = static_cast<double>((pos / 256) % 256);
        return m_noise_generator.get_int(static_cast<int>(min), static_cast<int>(max), x/100.f, y/100.f);
      }
      inline int get_random(double min, double max)
      {
        return static_cast<int>(min) + rand() % (( static_cast<int>(max) + 1 ) - static_cast<int>(min));
      }

  };
}
}
