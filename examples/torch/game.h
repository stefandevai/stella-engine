#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

namespace ex = entityx;

class Game : public ex::EntityX {
public:
  Game(stella::graphics::Display &display);
  ~Game();

  void Update(ex::TimeDelta dt);

private:
	stella::graphics::Display &Display;
	ex::Entity FPSText, Fire;

	std::unordered_map<std::string, stella::graphics::Texture*> Textures;
	std::unordered_map<std::string, stella::graphics::Texture*> Fonts;

	void LoadTexture(std::string tex_name, const char *tex_path);
	void LoadFont(std::string font_name, const char *font_path);

	void load_player(int x, int y);
	void load_blocks();
	void load_foreground();
	void load_text();
};
