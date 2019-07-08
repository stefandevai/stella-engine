#include "stella/scripting/lua_interface.h"
#include "stella/components.h"

namespace stella
{
namespace script
{
  LuaInterface::LuaInterface(entt::registry &registry)
    : m_registry(registry)
  {
    m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
    m_lua.set_function("create_layer", &LuaInterface::create_layer, this);
    m_lua.set_function("e_create_entity", &LuaInterface::create_entity, this);
    m_lua.set_function("e_add_component", &LuaInterface::add_component, this);
    m_lua.set_function("get_position", &LuaInterface::get_position, this);
    m_lua.set_function("get_perlin_int", &LuaInterface::get_perlin_int, this);
    m_lua.set_function("get_random_int", &LuaInterface::get_random, this);
  }

  LuaInterface::~LuaInterface()
  {

  }

  void LuaInterface::run_script(const std::string &script_path)
  {
    m_lua.script_file(script_path);
  }

  void LuaInterface::run_function(const std::string &function_name)
  {
    m_lua[function_name]();
  }

  std::tuple<int,int,int> LuaInterface::get_position(entt::registry::entity_type entity)
  {
    if (m_registry.has<stella::components::PositionComponent>(entity))
    {
      const auto &position = m_registry.get<stella::components::PositionComponent>(entity);
      return std::make_tuple(position.x, position.y, position.z);
    }
    else
    {
      std::cout << "Entity has no PositionComponent\n";
      return std::make_tuple(0,0,0);
    }
  }

  const entt::registry::entity_type LuaInterface::create_entity()
  {
    auto entity = m_registry.create();
    return entity;
  }

  void LuaInterface::create_layer(const sol::table &obj)
  {
    const std::string &layer_name = obj["name"] == sol::lua_nil ? std::string() : obj["name"];
    const unsigned &priority = obj["priority"] == sol::lua_nil ? 0 : obj["priority"];
    const std::string &shader_id = obj["shader"] == sol::lua_nil ? std::string("basic") : obj["shader"];
    const bool &fixed = obj["fixed"] == sol::lua_nil ? true : obj["fixed"];
    auto layer = m_registry.create();
    m_registry.assign<stella::components::LayerComponent>(layer, layer_name, priority, shader_id, fixed);
  }

  void LuaInterface::add_sprite_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const std::string &layer_id = obj["layer"] == sol::lua_nil ? std::string() : obj["layer"];
    const std::string &texture_name = obj["texture"] == sol::lua_nil ? std::string() : obj["texture"];

    if (!layer_id.empty() && !texture_name.empty())
    {
      if (obj["frame_dimensions"] == sol::lua_nil)
      {
        m_registry.assign<stella::components::SpriteComponent>(id, texture_name, layer_id);
      }
      else
      {
        const float &framew = obj["frame_dimensions"][1];
        const float &frameh = obj["frame_dimensions"][2];
        const unsigned &frame = obj["frame"] == sol::lua_nil ? 0 : obj["frame"];
        m_registry.assign<stella::components::SpriteComponent>(id, texture_name, glm::vec2(framew, frameh), layer_id, frame);
      }
    }
    else
    {
      std::cout << "You must provide a layer and a texture.\n";
    }
  }

  void LuaInterface::add_position_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const int &x = obj[1] == sol::lua_nil ? 0 : obj[1];
    const int &y = obj[2] == sol::lua_nil ? 0 : obj[2];
    const int &z = obj[3] == sol::lua_nil ? 0 : obj[3];
    m_registry.assign<stella::components::PositionComponent>(id, x, y, z);
  }

  void LuaInterface::add_dimension_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const unsigned w = obj[1] == sol::lua_nil ? 0 : obj[1];
    const unsigned h = obj[2] == sol::lua_nil ? 0 : obj[2];
    m_registry.assign<stella::components::DimensionComponent>(id, w, h);
  }
   
  void LuaInterface::add_animation_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const float &framew = obj["frame_dimensions"][1];
    const float &frameh = obj["frame_dimensions"][2];

    std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> animations;
    sol::table animations_obj = obj["animations"];
    for (const auto& key_value_pair : animations_obj)
    {
      const sol::table &animation = key_value_pair.second;
      const std::string &name = animation[1];
      const sol::table &frames_table = animation[2];
      const int &frames_table_size = frames_table.size();
      std::vector<unsigned int> frames(frames_table_size);
      for (int i = 1; i < frames_table_size+1; ++i)
      {
        int frame = frames_table[i];
        frames[i-1] = frame;
      }
      const unsigned int &speed = animation[3];
      assert(speed > 0);
      assert(frames.size() > 0);
      animations.emplace_back(name, frames, speed);
    }

    m_registry.assign<stella::components::AnimationsComponent>(id, animations, glm::vec2(framew, frameh));
  }

  void LuaInterface::add_tileview_component(entt::registry::entity_type id, const sol::table &obj)
  {
    m_registry.assign<stella::components::TileviewComponent>(id);
  }

  void LuaInterface::add_movement_component(entt::registry::entity_type id, const sol::table &obj)
  {
    glm::vec2 speed = obj["speed"] == sol::lua_nil ? glm::vec2() : glm::vec2(obj["speed"][1], obj["speed"][2]);
    const bool &gravity = obj["has_gravity"] == sol::lua_nil ? true : obj["has_gravity"];
    const bool &constant_velocity = obj["has_constant_velocity"] == sol::lua_nil ? false : obj["has_constant_velocity"];
    m_registry.assign<stella::components::MovementComponent>(id, speed, gravity, constant_velocity);
  }

  void LuaInterface::add_player_component(entt::registry::entity_type id, const sol::table &obj)
  {
    m_registry.assign<stella::components::PlayerComponent>(id);
  }

  void LuaInterface::add_body_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const bool &collide_with_borders = obj["collide_with_borders"] == sol::lua_nil ? false : obj["collide_with_borders"];
    if (obj["drag"] == sol::lua_nil)
    {
      m_registry.assign<stella::components::Body2DComponent>(id);
    }
    else
    {
      double dragx = obj["drag"][1];
      double dragy = obj["drag"][2];
      m_registry.assign<stella::components::Body2DComponent>(id, std::vector<double>(dragx, dragy), collide_with_borders);
    }
  }

  void LuaInterface::add_text_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const std::string &text = obj["text"];
    const std::string &font_name = obj["font_name"];
    const bool &is_static = obj["is_static"];
    m_registry.assign<stella::components::TextComponent>(id, text, font_name, is_static);
  }

  void LuaInterface::add_particle_emitter_component(entt::registry::entity_type id, const sol::table &obj)
  {
    const std::string &type = obj["type"];
    const unsigned int &quantity = obj["quantity"];
    std::cout << type << '\n';
    std::cout << quantity << '\n';
    stella::components::ParticleEmitter::Type emitter_type;
    if (type == "fire")
    {
      emitter_type = stella::components::ParticleEmitter::Type::FIRE_EMITTER;
    }
    else if (type == "snow")
    {
      emitter_type = stella::components::ParticleEmitter::Type::SNOW_EMITTER;
    }
    m_registry.assign<stella::components::ParticleEmitter>(id, emitter_type, quantity);
  }

  void LuaInterface::add_tile_component(entt::registry::entity_type id, const sol::table &obj)
  {
    m_registry.assign<stella::components::TileComponent>(id);
  }

  void LuaInterface::add_scroll_component(entt::registry::entity_type id, const sol::table &obj)
  {
    glm::vec2 speed = obj == sol::lua_nil ? glm::vec2(0.f, 0.f) : glm::vec2(obj[1], obj[2]);
    m_registry.assign<stella::components::ScrollComponent>(id, speed);
  }

  void LuaInterface::add_component(const sol::table& obj)
  {
    if (obj["type"] != sol::lua_nil)
    {
      const std::string &ct = obj["type"];
      entt::registry::entity_type id = obj["id"];

      if (ct == "sprite") add_sprite_component(id, obj["args"]);
      else if (ct == "position") add_position_component(id, obj["args"]);
      else if (ct == "dimension") add_dimension_component(id, obj["args"]);
      else if (ct == "animation") add_animation_component(id, obj["args"]);
      else if (ct == "tile") add_tile_component(id, obj["args"]);
      else if (ct == "body") add_body_component(id, obj["args"]);
      else if (ct == "text") add_text_component(id, obj["args"]);
      else if (ct == "movement") add_movement_component(id, obj["args"]);
      else if (ct == "tileview") add_tileview_component(id, obj["args"]);
      else if (ct == "particle_emitter") add_particle_emitter_component(id, obj["args"]);
      else if (ct == "scroll") add_scroll_component(id, obj["args"]);
      else if (ct == "player") add_player_component(id, obj["args"]);
      else std::cout << "ERROR: No component named " << ct << '\n';
    }
    else
    {
      std::cout << "ERROR: Please add a non nil component type\n";
    }
  }
}
}

