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
		std::map<std::string, std::vector<unsigned int>> animations;
};

