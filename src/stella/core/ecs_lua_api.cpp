#include "stella/core/ecs_lua_api.hpp"
#include "stella/components.hpp" // IWYU pragma: export
#include "stella/components/color.hpp"
#include "stella/components/sprite.hpp"
#include "stella/components/layer.hpp"
#include "stella/components/tileview.hpp"
#include "stella/components/movement.hpp"
#include "stella/components/player.hpp"
#include "stella/components/scroll.hpp"
#include "stella/components/name.hpp"
#include "stella/components/animation_player.hpp"
#include "stella/components/vertical.hpp"
// #include "stella/components/particle_emitter.hpp"
#include <assert.h>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <spdlog/spdlog.h>

namespace stella
{
namespace script
{
  ECSLuaApi::ECSLuaApi (entt::registry& registry) : m_registry (registry)
  {
    m_lua.open_libraries (sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
    m_lua.set_function ("create_layer", &ECSLuaApi::create_layer, this);
    m_lua.set_function ("e_create_entity", &ECSLuaApi::create_entity, this);
    m_lua.set_function ("e_add_component", &ECSLuaApi::add_component, this);
    m_lua.set_function ("get_position", &ECSLuaApi::get_position, this);
    m_lua.set_function ("get_perlin_int", &ECSLuaApi::get_perlin_int, this);
    m_lua.set_function ("get_random_int", &ECSLuaApi::get_random, this);
  }

  std::tuple<int, int, int> ECSLuaApi::get_position (entt::registry::entity_type entity)
  {
    if (m_registry.has<stella::component::Position> (entity))
    {
      const auto& position = m_registry.get<stella::component::Position> (entity);
      return std::make_tuple (position.x, position.y, position.z);
    }
    else
    {
      spdlog::warn ("Entity has no Position");
      return std::make_tuple (0, 0, 0);
    }
  }

  const entt::registry::entity_type ECSLuaApi::create_entity()
  {
    auto entity = m_registry.create();
    return entity;
  }

  void ECSLuaApi::create_layer (const sol::table& obj)
  {
    const std::string& layer_name  = obj["name"] == sol::lua_nil ? std::string() : obj["name"];
    const unsigned& priority       = obj["priority"] == sol::lua_nil ? 0 : obj["priority"];
    const std::string& shader_id   = obj["shader"] == sol::lua_nil ? std::string ("basic") : obj["shader"];
    std::string vert_shader_source = obj["vert_source"] == sol::lua_nil ? std::string() : obj["vert_source"];
    std::string frag_shader_source = obj["frag_source"] == sol::lua_nil ? std::string() : obj["frag_source"];
    const bool& fixed              = obj["fixed"] == sol::lua_nil ? true : obj["fixed"];
    auto layer                     = m_registry.create();

    component::LayerType layer_type = component::LayerType::SPRITE_LAYER;

    if (shader_id == "shape")
    {
      layer_type = component::LayerType::SHAPE_LAYER;
    }

    m_registry.emplace<stella::component::LayerT> (layer, layer_name, priority, layer_type, vert_shader_source, frag_shader_source, fixed);
  }

  void ECSLuaApi::add_sprite_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const std::string& layer_id     = obj["layer"] == sol::lua_nil ? std::string() : obj["layer"];
    const std::string& texture_name = obj["texture"] == sol::lua_nil ? std::string() : obj["texture"];
    const unsigned& frame           = obj["frame"] == sol::lua_nil ? 0 : obj["frame"];

    auto& sprite = m_registry.emplace<stella::component::SpriteT> (id, texture_name);
    sprite.frame = frame;
    sprite.layer = layer_id;
  }

  void ECSLuaApi::add_position_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const int& x = obj[1] == sol::lua_nil ? 0 : obj[1];
    const int& y = obj[2] == sol::lua_nil ? 0 : obj[2];
    const int& z = obj[3] == sol::lua_nil ? 0 : obj[3];
    m_registry.emplace<stella::component::Position> (id, x, y, z);
  }

  void ECSLuaApi::add_dimension_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const unsigned w = obj[1] == sol::lua_nil ? 0 : obj[1];
    const unsigned h = obj[2] == sol::lua_nil ? 0 : obj[2];
    m_registry.emplace<stella::component::Dimension> (id, w, h);
  }

  void ECSLuaApi::add_animation_component (entt::registry::entity_type id, const sol::table& obj)
  {
    auto& anim_player = m_registry.emplace<stella::component::AnimationPlayer> (id);
    // anim_player.loop  = obj["loop"] == sol::nil ? false : obj["loop"];

    sol::table animations_obj = obj["animations"];
    for (const auto& key_value_pair : animations_obj)
    {
      const sol::table& animation    = key_value_pair.second;
      const std::string& name        = animation[1];
      const sol::table& frames_table = animation[2];
      const int frames_table_size    = frames_table.size();
      component::AnimationData data;
      data.frames.reserve (frames_table_size);
      for (int i = 1; i <= frames_table_size; ++i)
      {
        data.frames.push_back (frames_table[i]);
      }

      data.step = animation[3] == sol::nil ? 0.1f : animation[3];
      assert (data.step > 0);
      assert (data.frames.size() > 0);

      anim_player.add (name, data);
    }
  }

  void ECSLuaApi::add_tileview_component (entt::registry::entity_type id, const sol::table& obj) { m_registry.emplace<stella::component::Tileview> (id); }

  void ECSLuaApi::add_movement_component (entt::registry::entity_type id, const sol::table& obj)
  {
    glm::vec2 speed               = obj["speed"] == sol::lua_nil ? glm::vec2() : glm::vec2 (obj["speed"][1], obj["speed"][2]);
    const bool& gravity           = obj["has_gravity"] == sol::lua_nil ? true : obj["has_gravity"];
    const bool& constant_velocity = obj["has_constant_velocity"] == sol::lua_nil ? false : obj["has_constant_velocity"];
    m_registry.emplace<stella::component::Movement> (id, speed, gravity, constant_velocity);
  }

  void ECSLuaApi::add_player_component (entt::registry::entity_type id, const sol::table& obj) { m_registry.emplace<stella::component::Player> (id); }

  void ECSLuaApi::add_transform_component (entt::registry::entity_type id, const sol::table& obj)
  {
    glm::vec3 rotation{0.f, 0.f, 0.f}, scale{0.f, 0.f, 0.f};

    const sol::table rotation_table = obj["rotation"];
    if (rotation_table != sol::nil)
    {
      if (rotation_table.size() == 3)
      {
        rotation.x = rotation_table[1];
        rotation.y = rotation_table[2];
        rotation.z = rotation_table[3];
      }
    }

    const sol::table scale_table = obj["scale"];
    if (scale_table != sol::nil)
    {
      if (scale_table.size() == 3)
      {
        scale.x = scale_table[1];
        scale.y = scale_table[2];
        scale.z = scale_table[3];
      }
    }

    m_registry.emplace<stella::component::Transform> (id, rotation, scale);
  }

  void ECSLuaApi::add_body_component (entt::registry::entity_type id, const sol::table& obj)
  {
    std::vector<double> bb    = {0.f, 0.f};
    std::vector<double> bbpos = {0.f, 0.f};
    const int movement_speed  = obj["movement_speed"] == sol::lua_nil ? 20.f : obj["movement_speed"];

    if (obj["bounding_box"] != sol::lua_nil)
    {
      bb[0] = obj["bounding_box"][1];
      bb[1] = obj["bounding_box"][2];
    }
    if (obj["bounding_box_position"] != sol::lua_nil)
    {
      bbpos[0] = obj["bounding_box_position"][1];
      bbpos[1] = obj["bounding_box_position"][2];
    }
    m_registry.emplace<stella::component::Body2D> (id, bb, bbpos, movement_speed);
  }

  void ECSLuaApi::add_text_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const std::wstring& text     = obj["text"];
    const std::string& font_name = obj["font_name"];
    const std::string& color     = obj["color"] == sol::lua_nil ? std::string ("#ffffffff") : obj["color"];
    const bool& is_static        = obj["is_static"] == sol::lua_nil ? true : obj["is_static"];
    m_registry.emplace<stella::component::Text> (id, text, font_name, color, is_static);
  }

  // void ECSLuaApi::add_particle_emitter_component (entt::registry::entity_type id, const sol::table& obj)
  // {
  //   const std::string& type                               = obj["type"];
  //   const unsigned int quantity                           = obj["quantity"];
  //   stella::component::ParticleEmitter::Type emitter_type = stella::component::ParticleEmitter::FIRE_EMITTER;
  //   if (type == "fire")
  //   {
  //     emitter_type = stella::component::ParticleEmitter::Type::FIRE_EMITTER;
  //   }
  //   else if (type == "snow")
  //   {
  //     emitter_type = stella::component::ParticleEmitter::Type::SNOW_EMITTER;
  //   }
  //   m_registry.emplace<stella::component::ParticleEmitter> (id, emitter_type, quantity);
  // }

  void ECSLuaApi::add_tile_component (entt::registry::entity_type id, const sol::table& obj) { m_registry.emplace<stella::component::Tile> (id, 0, false); }

  void ECSLuaApi::add_scroll_component (entt::registry::entity_type id, const sol::table& obj)
  {
    glm::vec2 speed = obj == sol::lua_nil ? glm::vec2 (0.f, 0.f) : glm::vec2 (obj[1], obj[2]);
    m_registry.emplace<stella::component::Scroll> (id, speed);
  }

  void ECSLuaApi::add_name_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const std::string& name = obj == sol::lua_nil ? std::string() : obj[1];
    m_registry.emplace<stella::component::Name> (id, name);
  }

  void ECSLuaApi::add_npc_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const std::string& path = obj["script_path"] == sol::lua_nil ? std::string() : obj["script_path"];
    m_registry.emplace<stella::component::NPC> (id, path);
  }

  void ECSLuaApi::add_character_animation_component (entt::registry::entity_type id) { m_registry.emplace<stella::component::CharacterAnimation> (id); }

  void ECSLuaApi::add_shape_component (entt::registry::entity_type id, const sol::table& obj)
  {
    const sol::table& verts     = obj["vertices"];
    const std::string& layer_id = obj["layer"] == sol::lua_nil ? std::string() : obj["layer"];
    const int verts_size        = verts.size();
    std::vector<glm::vec3> vertices (verts_size);
    for (int i = 1; i <= verts_size; ++i)
    {
      const sol::table& v = verts[i];
      assert (v.size() == 3);
      vertices[i - 1] = glm::vec3{v[1], v[2], v[3]};
    }
    m_registry.emplace<stella::component::Shape> (id, vertices, layer_id);
  }

  void ECSLuaApi::add_color_component (entt::registry::entity_type id, const sol::table& obj)
  {
    if (obj["rgba"] != sol::lua_nil)
    {
      const sol::table& color = obj["rgba"];
      m_registry.emplace<stella::component::Color> (id, color[1], color[2], color[3], color[4]);
    }
    else if (obj["rgb"] != sol::lua_nil)
    {
      const sol::table& color = obj["rgb"];
      m_registry.emplace<stella::component::Color> (id, color[1], color[2], color[3]);
    }
    else if (obj["hex"] != sol::lua_nil)
    {
      const std::string& hex_str = obj["hex"];
      m_registry.emplace<stella::component::Color> (id, hex_str);
    }
  }

  void ECSLuaApi::add_vertical_component (entt::registry::entity_type id) { m_registry.emplace<stella::component::Vertical> (id); }

  void ECSLuaApi::add_component (const sol::table& obj)
  {
    if (obj["type"] != sol::lua_nil)
    {
      const std::string& ct          = obj["type"];
      entt::registry::entity_type id = obj["id"];

      // TODO: Change for a map of strings and functions
      if (ct == "sprite")
        add_sprite_component (id, obj["args"]);
      else if (ct == "position")
        add_position_component (id, obj["args"]);
      else if (ct == "dimension")
        add_dimension_component (id, obj["args"]);
      else if (ct == "animation")
        add_animation_component (id, obj["args"]);
      else if (ct == "tile")
        add_tile_component (id, obj["args"]);
      else if (ct == "body")
        add_body_component (id, obj["args"]);
      else if (ct == "text")
        add_text_component (id, obj["args"]);
      else if (ct == "movement")
        add_movement_component (id, obj["args"]);
      else if (ct == "tileview")
        add_tileview_component (id, obj["args"]);
      // else if (ct == "particle_emitter")
      //   add_particle_emitter_component (id, obj["args"]);
      else if (ct == "scroll")
        add_scroll_component (id, obj["args"]);
      else if (ct == "player")
        add_player_component (id, obj["args"]);
      else if (ct == "name")
        add_name_component (id, obj["args"]);
      else if (ct == "transform")
        add_transform_component (id, obj["args"]);
      else if (ct == "npc")
        add_npc_component (id, obj["args"]);
      else if (ct == "character_animation")
        add_character_animation_component (id);
      else if (ct == "shape")
        add_shape_component (id, obj["args"]);
      else if (ct == "color")
        add_color_component (id, obj["args"]);
      else if (ct == "vertical")
        add_vertical_component (id);
      else
        spdlog::warn ("No component named " + ct);
    }
    else
    {
      spdlog::warn ("Component type is nil");
    }
  }
} // namespace script
} // namespace stella
