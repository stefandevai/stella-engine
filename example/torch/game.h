#pragma once

#include <unordered_map>
#include <string>

#include <entityx/entityx.h>
#include <stella/stella.h>
#include <stella/components/game_components.h>

#define SOL_ALL_SAFETIES_ON 1
#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

namespace ex = entityx;


class Game : public ex::EntityX {
public:
  Game(stella::graphics::Display &display);
  ~Game();

  void Update(ex::TimeDelta dt);

private:
  enum States {NONE, GAME_LOADING, GAME_LOADED, GAME_NOT_LOADED, MAIN_MENU, GAME_LOOP, GAME_PAUSED, GAME_OVER};
	stella::graphics::Display &Display;
	ex::Entity FPSText, Fire;
  sol::state lua;
  stella::ScriptApi scriptApi;

  ex::Entity layer1, layer2, layer3;

	std::unordered_map<std::string, stella::graphics::Texture*> Textures;
	std::unordered_map<std::string, stella::graphics::Texture*> Fonts;

  typedef void (*AddComponentFunction)(sol::table);
  std::unordered_map<std::string, AddComponentFunction> function_map;

  void create_function_map();

	void LoadTexture(std::string tex_name, const char *tex_path);
	void LoadFont(std::string font_name, const char *font_path);

	void load_player(int x, int y);
	void load_blocks();
	void load_particles();
	void load_background();
	void load_foreground();
	void load_game_info();

  void update_systems(const double &dt);

  inline const std::tuple<unsigned int, unsigned int> create_entity()
  {
    auto entity = entities.create();
    auto eid = entity.id();
    return std::tuple<unsigned int, unsigned int>(eid.index(), eid.version());
  }

  inline void add_sprite_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const std::string layer_id = obj["layer"] == sol::lua_nil ? std::string() : obj["layer"];
    const std::string &texture_name = obj["texture"] == sol::lua_nil ? std::string() : obj["texture"];

    if (!layer_id.empty() && !texture_name.empty())
    {
      if (obj["frame_dimensions"] == sol::lua_nil)
      {
        entities.assign<stella::components::SpriteComponent>(ex::Entity::Id(index, version), texture_name, layer_id);
      }
      else
      {
        const float &framew = obj["frame_dimensions"][1];
        const float &frameh = obj["frame_dimensions"][2];
        entities.assign<stella::components::SpriteComponent>(ex::Entity::Id(index, version), texture_name, glm::vec2(framew, frameh), layer_id);
      }
    }
    else
    {
      std::cout << "You must provide a layer and a texture.\n";
    }
  }

  inline void add_position_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const int &x = obj[1] == sol::lua_nil ? 0 : obj[1];
    const int &y = obj[2] == sol::lua_nil ? 0 : obj[2];
    const int &z = obj[3] == sol::lua_nil ? 0 : obj[3];
    entities.assign<stella::components::PositionComponent>(ex::Entity::Id(index, version), x, y, z);
  }

  inline void add_dimension_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const unsigned w = obj[1] == sol::lua_nil ? 0 : obj[1];
    const unsigned h = obj[2] == sol::lua_nil ? 0 : obj[2];
    entities.assign<stella::components::DimensionComponent>(ex::Entity::Id(index, version), w, h);
  }
   
  inline void add_animation_component(const unsigned &index, const unsigned &version, const sol::table &obj)
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

    std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> moon_anims;
    moon_anims.emplace_back("glow", std::vector<unsigned int>{3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20);
    entities.assign<stella::components::AnimationsComponent>(ex::Entity::Id(index, version), animations, glm::vec2(framew, frameh));
  }

  inline void add_tileview_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    entities.assign<stella::components::TileviewComponent>(ex::Entity::Id(index, version));
  }

  inline void add_movement_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    glm::vec2 speed = obj["speed"] == sol::lua_nil ? glm::vec2() : glm::vec2(obj["speed"][1], obj["speed"][2]);
    const bool &gravity = obj["has_gravity"] == sol::lua_nil ? true : obj["has_gravity"];
    const bool &constant_velocity = obj["has_constant_velocity"] == sol::lua_nil ? false : obj["has_constant_velocity"];
    entities.assign<stella::components::MovementComponent>(ex::Entity::Id(index, version), speed, gravity, constant_velocity);
  }

  inline void add_player_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    entities.assign<stella::components::PlayerComponent>(ex::Entity::Id(index, version));
  }

  inline void add_body_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const bool &collide_with_borders = obj["collide_with_borders"] == sol::lua_nil ? false : obj["collide_with_borders"];
    if (obj["drag"] == sol::lua_nil)
    {
      entities.assign<stella::components::Body2DComponent>(ex::Entity::Id(index, version));
    }
    else
    {
      double dragx = obj["drag"][1];
      double dragy = obj["drag"][2];
      entities.assign<stella::components::Body2DComponent>(ex::Entity::Id(index, version), std::vector<double>(dragx, dragy), collide_with_borders);
    }
  }

  inline void add_text_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const std::string &text = obj["text"];
    const std::string &font_name = obj["font_name"];
    const bool &is_static = obj["is_static"];
    entities.assign<stella::components::TextComponent>(ex::Entity::Id(index, version), text, font_name, is_static);
  }

  inline void add_particle_emitter_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    const std::string &type = obj["type"];
    const unsigned int &quantity = obj["quantity"];
    stella::components::ParticleEmitter::Type emitter_type;
    if (type == "fire")
    {
      emitter_type = stella::components::ParticleEmitter::Type::FIRE_EMITTER;
    }
    else if (type == "snow")
    {
      emitter_type = stella::components::ParticleEmitter::Type::SNOW_EMITTER;
    }
    entities.assign<stella::components::ParticleEmitter>(ex::Entity::Id(index, version), emitter_type, quantity);
  }

  inline void add_tile_component(const unsigned &index, const unsigned &version, const sol::table &obj)
  {
    entities.assign<stella::components::TileComponent>(ex::Entity::Id(index, version));
  }
  
  void add_component(const sol::table &obj);
};

