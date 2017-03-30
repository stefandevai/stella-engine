#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys) {
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.add<PlayerMovementSystem>((int)display.GetWidth(), (int)display.GetHeight());
	systems.add<LightingSystem>(shader);
	systems.add<AnimationSystem>();
	systems.configure();

	// Background
	entityx::Entity sky = entities.create();
	entityx::Entity moon = entities.create();
	SkyTex = new stella::graphics::Texture("sky-tex", "assets/gfx/sprites/sky_background.png");
	MoonTex = new stella::graphics::Texture("moon-tex", "assets/gfx/sprites/moon_anim.png");

	sky.assign<PositionComponent>(0, 0);
	sky.assign<TextureComponent>(720, 405, *SkyTex, 0);
	
	moon.assign<PositionComponent>(438, 98);
	moon.assign<TextureComponent>(85, 85, *MoonTex, 0);

	add_animation(moon, "moon", { 3,0,4,2,1,4,3,0,2,4,3 }, 20);
	moon.assign<AnimationComponent>("moon");

	// Player
	entityx::Entity player = entities.create();
	PlayerTex = new stella::graphics::Texture("guanaco-tex", "assets/gfx/sprites/guanaco-anim.png");

	player.assign<PositionComponent>((int)display.GetWidth()/2 - 40, (int)display.GetHeight()/2 - 30);
	player.assign<TextureComponent>(80, 60, *PlayerTex, 0);
	player.assign<InputComponent>(keys);
	player.assign<LightComponent>(0, 1.0f);

	add_animation(player, "run", { 0,1,2,3,4 }, 5);
	player.assign<AnimationComponent>("run");

	// Terrain
	entityx::Entity block = entities.create();
	entityx::Entity over_block = entities.create();
	BlockTex = new stella::graphics::Texture("block-tex", "assets/gfx/sprites/block.png");
	OverBlockTex = new stella::graphics::Texture("details-tex", "assets/gfx/sprites/over_block.png");

	block.assign<PositionComponent>(0, 313);
	block.assign<TextureComponent>(720, 92, *BlockTex, 0);

	over_block.assign<PositionComponent>(0, 301);
	over_block.assign<TextureComponent>(720, 12, *OverBlockTex, 0);
}

Game::~Game() {
	delete PlayerTex;
	delete SkyTex;
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

