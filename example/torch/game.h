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

	std::unordered_map<std::string, stella::graphics::Texture*> Textures;
	std::unordered_map<std::string, stella::graphics::Texture*> Fonts;

	void LoadTexture(std::string tex_name, const char *tex_path);
	void LoadFont(std::string font_name, const char *font_path);

	void load_player(int x, int y);
	void load_blocks();
	void load_particles();
	void load_background();
	void load_foreground();
	void load_text();

  void update_systems(const double &dt);

  inline const std::tuple<unsigned int, unsigned int> create_entity()
  {
    auto entity = entities.create();
    auto eid = entity.id();
    return std::tuple<unsigned int, unsigned int>(eid.index(), eid.version());
  }

  inline void add_sprite_component(const sol::table &obj)
  {
    const int &type = obj["type"];
    const int &index = obj["index"];
    const int &version = obj["version"];
    const std::string &texture_name = obj["texture_name"];

    if (type == 0)
    {
      entities.assign<stella::components::SpriteComponent>(ex::Entity::Id(index, version), texture_name);
    }
    else if (type == 1)
    {
      const float &framew = obj["frame_dimensions"][1];
      const float &frameh = obj["frame_dimensions"][2];
      entities.assign<stella::components::SpriteComponent>(ex::Entity::Id(index, version), texture_name, glm::vec2(framew, frameh));
    }
  }

  inline void add_dimension_component(const unsigned int &index, const unsigned int &version, const unsigned int w, const unsigned int h)
  {
    entities.assign<stella::components::DimensionComponent>(ex::Entity::Id(index, version), w, h);
  }

  inline void add_position_component(const unsigned int &index, const unsigned int &version, const int x, const int y)
  {
    entities.assign<stella::components::PositionComponent>(ex::Entity::Id(index, version), x, y);
  }
   
  inline void add_animation_component(const sol::table &obj)
  {
    const int &index = obj["index"];
    const int &version = obj["version"];
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
};
