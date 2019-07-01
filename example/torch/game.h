#pragma once

#include <unordered_map>
#include <string>

#include <entityx/entityx.h>
#include <stella/stella.h>
#include <stella/components.h>

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
	ex::Entity FPSText, Fire, camera;
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

  // Lua functions
  void create_layer(const sol::table &obj);
  const std::tuple<unsigned int, unsigned int> create_entity();
  void add_sprite_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_position_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_dimension_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_animation_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_tileview_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_movement_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_player_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_body_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_text_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_particle_emitter_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_tile_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_scroll_component(const unsigned &index, const unsigned &version, const sol::table &obj);
  void add_component(const sol::table &obj);
};

