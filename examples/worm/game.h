#pragma once

#include <string>
#include <vector>
#include <map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

#include "snake.h"

class Game : public entityx::EntityX {
	public:
		Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys);
		~Game();

		void Update(entityx::TimeDelta dt);

	private:
		Snake *snake;
		stella::graphics::Texture *CoinTex;

		void add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate);
		void AddCoins(size_t num_coins);
};

