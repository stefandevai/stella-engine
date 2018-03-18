#pragma once

#include <string>
#include <vector>
#include <map>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

struct Snake {
	entityx::Entity head;
	std::vector<entityx::Entity*> body;
};

class Game : public entityx::EntityX {
	public:
		Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys);
		~Game();

		void Update(entityx::TimeDelta dt);

	private:
		Snake snake;
		stella::graphics::Texture *SnakeTex;
		void add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate);
		void update_snake();
};

