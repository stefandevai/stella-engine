#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys) {
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.add<PlayerMovementSystem>((int)display.GetWidth(), (int)display.GetHeight());
	systems.add<LightingSystem>(shader);
	systems.add<AnimationSystem>();
	systems.configure();
		
	entityx::Entity player = entities.create();

	player.assign<PositionComponent>((int)display.GetWidth()/2 - 80, (int)display.GetHeight()/2 - 60);

	PlayerTex = new stella::graphics::Texture("guanaco-tex", "assets/gfx/sprites/guanaco-anim.png");
	player.assign<TextureComponent>(160, 120, *PlayerTex, 0);

	player.assign<InputComponent>(keys);

	player.assign<LightComponent>(0, 0.8f);
	
	add_animation(player, "run", { 0,1,2,3,4 }, 5);
	player.assign<AnimationComponent>("run");
}

Game::~Game() {
	delete PlayerTex;
}

void Game::add_animation(entityx::Entity &ent, std::string name, std::vector<unsigned int> frames, unsigned int framerate) {
	entityx::ComponentHandle<TextureComponent> tex = ent.component<TextureComponent>();
			std::cout << frames.size() << std::endl;
	if (tex) {
		tex->sprite->Animations.Add(name, frames, framerate);
	}
}

void Game::Update(entityx::TimeDelta dt) {
	systems.update_all(dt);
}

