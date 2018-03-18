#pragma once

#include <string>
#include <vector>
#include <map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

class Game : public entityx::EntityX {
	public:
		Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys);
		~Game();

		void Update(entityx::TimeDelta dt);

	private:
		stella::graphics::Texture *PlayerTex;
		stella::graphics::Texture *SkyTex;
		stella::graphics::Texture *MoonTex;
		stella::graphics::Texture *Mou1;
		stella::graphics::Texture *Mou2;
		stella::graphics::Texture *Mou3;
		stella::graphics::Texture *BlockTex;
		stella::graphics::Texture *OverBlockTex;
		void add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate);
};

