#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

class Game : public entityx::EntityX {
public:
  Game(stella::graphics::Display &display);
  ~Game();

  void Update(entityx::TimeDelta dt);

private:
	std::unordered_map<std::string, stella::graphics::Texture*> Textures;
  stella::graphics::Texture *PlayerTex;
  stella::graphics::Texture *SkyTex;
  stella::graphics::Texture *MoonTex;
  stella::graphics::Texture *Mou1;
  stella::graphics::Texture *Mou2;
  stella::graphics::Texture *Mou3;
  stella::graphics::Texture *BlockTex;
  stella::graphics::Texture *OverBlockTex;
  void add_animation(entityx::Entity &ent, std::string name,
                     std::vector<unsigned int> frames, unsigned int framerate);

	void LoadTexture(std::string tex_name, const char *tex_path);
};
