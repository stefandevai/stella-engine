#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys) {
	systems.add<CollisionSystem>((int)display.GetWidth(), (int)display.GetHeight());
	systems.add<MovementSystem>();
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.add<PlayerMovementSystem>((int)display.GetWidth());
	systems.add<LightingSystem>(shader);
	systems.add<AnimationSystem>();
	systems.configure();

	snake = new Snake(entities, keys);
}

Game::~Game() {
	delete snake;
}

void Game::add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate) {
	entityx::ComponentHandle<TextureComponent> tex = ent.component<TextureComponent>();
	if (tex) {
		tex->sprite->Animations.Add(name, frames, framerate);
	}
}

void Game::Update(entityx::TimeDelta dt) {
	systems.update_all(dt);
}

