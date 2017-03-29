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
	
	std::vector<unsigned int> run_frames = { 0,1,2,3,4 };
	animations.insert(std::make_pair("run", run_frames));
	player.assign<AnimationComponent>("run", animations);
}

Game::~Game() {
	delete PlayerTex;
}

void Game::Update(entityx::TimeDelta dt) {
	systems.update_all(dt);
}

