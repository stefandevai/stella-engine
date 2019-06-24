#pragma once

#include <unordered_map>
#include <string>

#include <entityx/entityx.h>
#include <stella/stella.h>

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
};
