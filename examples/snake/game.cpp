#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys) {
	systems.add<CollisionSystem>((int)display.GetWidth(), (int)display.GetHeight());
	systems.add<MovementSystem>();
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.add<PlayerMovementSystem>((int)display.GetWidth());
	systems.add<LightingSystem>(shader);
	systems.add<AnimationSystem>();
	systems.configure();

	SnakeTex = new stella::graphics::Texture("snake-tex", "assets/gfx/snake/snake.png");
	snake = entities.create();

	snake.assign<TextureComponent>(32, 32, *SnakeTex, 0);
	snake.assign<SpatialComponent>(32, 32, 320, 320);
	snake.assign<MovementComponent>(3, 40);
	snake.assign<TimerComponent>();
	snake.assign<InputComponent>(keys);
}

Game::~Game() {
	delete SnakeTex;
}

void Game::add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate) {
	entityx::ComponentHandle<TextureComponent> tex = ent.component<TextureComponent>();
	if (tex) {
		tex->sprite->Animations.Add(name, frames, framerate);
	}
}

void Game::update_snake() {

}

void Game::Update(entityx::TimeDelta dt) {
	systems.update_all(dt);
	this->update_snake();
}

