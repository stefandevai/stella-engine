#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader) {
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.configure();
		
	entityx::Entity player = entities.create();
	player.assign<Position>((int)display.GetWidth()/2 - 80, (int)display.GetHeight()/2 - 60);

	PlayerTex = new stella::graphics::Texture("guanaco-tex", "assets/gfx/sprites/guanaco-anim.png");
	player.assign<TextureComponent>(160, 120, *PlayerTex, 0);
}

Game::~Game() {
	delete PlayerTex;
}

void Game::Update(entityx::TimeDelta dt) {
	systems.update_all(dt);
}

