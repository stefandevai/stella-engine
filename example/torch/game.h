#pragma once

#include <unordered_map>
#include <string>

#include <entityx/entityx.h>
#include <stella/stella.h>
#include <stella/components/game_components.h>

#define SOL_ALL_SAFETIES_ON 1
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

  inline void add_sprite_component(const unsigned int &index, const unsigned int &version, const std::string texture_name)
  {
    //auto entity = entities.get(ex::Entity::Id(index, version));
    //entity.assign<stella::components::SpriteComponent>(texture_name);
    entities.assign<stella::components::SpriteComponent>(ex::Entity::Id(index, version), texture_name);
  }

  inline void add_dimension_component(const unsigned int &index, const unsigned int &version, const unsigned int w, const unsigned int h)
  {
    //auto entity = entities.get(ex::Entity::Id(index, version));
    //entity.assign<stella::components::DimensionComponent>(w, h);
    entities.assign<stella::components::DimensionComponent>(ex::Entity::Id(index, version), w, h);
  }

  inline void add_position_component(const unsigned int &index, const unsigned int &version, const int x, const int y)
  {
    //auto entity = entities.get(ex::Entity::Id(index, version));
    //entity.assign<stella::components::PositionComponent>(x, y);
    entities.assign<stella::components::PositionComponent>(ex::Entity::Id(index, version), x, y);
  }
};
