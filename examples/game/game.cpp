#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader, const bool *keys) {
	systems.add<CollisionSystem>((int)display.GetWidth(), (int)display.GetHeight());
	systems.add<MovementSystem>();
	systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(), shader);
	systems.add<PlayerMovementSystem>((int)display.GetWidth());
	systems.add<LightingSystem>(shader);
	systems.add<AnimationSystem>();
	systems.add<TileviewSystem>((int)display.GetWidth());
	systems.configure();

	// Background
	entityx::Entity sky = entities.create();
	entityx::Entity moon = entities.create();
	SkyTex = new stella::graphics::Texture("sky-tex", "assets/gfx/sprites/sky_background.png");
	MoonTex = new stella::graphics::Texture("moon-tex", "assets/gfx/sprites/moon_anim.png");

	sky.assign<TextureComponent>(720, 405, *SkyTex, 0);
	sky.assign<SpatialComponent>(720, 405);
	
	moon.assign<TextureComponent>(85, 85, *MoonTex, 0);
	moon.assign<SpatialComponent>(85, 85, 478, 78);

	add_animation(moon, "moon", { 3,0,4,2,1,4,3,0,2,4,3 }, 20);
	moon.assign<AnimationComponent>("moon");

	// Player
	entityx::Entity player = entities.create();
	PlayerTex = new stella::graphics::Texture("guanaco-tex", "assets/gfx/sprites/guanaco-anim.png");
	//PlayerTex = new stella::graphics::Texture("guanaco-tex", "assets/gfx/sprites/black_block.png");

	//player.assign<BodyComponent>(64, 64, 0, 0, false);
	player.assign<BodyComponent>(80, 60, 0, 0, false);
	player.assign<MovementComponent>(0.7f, 8.0f, 1.5f);
	//player.assign<TextureComponent>(64, 64, *PlayerTex, 0);
	player.assign<TextureComponent>(80, 60, *PlayerTex, 0);
	player.assign<SpatialComponent>(80, 60, 140, 250);
	player.assign<InputComponent>(keys);
	player.assign<LightComponent>(0, 1.0f);

	add_animation(player, "run", { 0,1,2,3,4 }, 5);
	player.assign<AnimationComponent>("run");


	// Terrain
	entityx::Entity block = entities.create();
	entityx::Entity over_block = entities.create();
	entityx::Entity over_block2 = entities.create();
	BlockTex = new stella::graphics::Texture("block-tex", "assets/gfx/sprites/block.png");
	OverBlockTex = new stella::graphics::Texture("details-tex", "assets/gfx/sprites/over_block.png");

	//block.assign<PositionComponent>(200, 113);
	block.assign<BodyComponent>(720, 92, 0, 0, true);
	block.assign<TextureComponent>(720, 92, *BlockTex, 0);
	block.assign<SpatialComponent>(720, 92, 0, 313);

	over_block.assign<TextureComponent>(720, 12, *OverBlockTex, 0);
	over_block.assign<SpatialComponent>(720, 12, 0, 301);
	over_block.assign<MovementComponent>(-3.0f, 0.0f, 3.0f, false);
	over_block.assign<TileviewComponent>();

	over_block2.assign<TextureComponent>(720, 12, *OverBlockTex, 0);
	over_block2.assign<SpatialComponent>(720, 12, 720, 301);
	over_block2.assign<MovementComponent>(-3.0f, 0.0f, 3.0f, false);
	over_block2.assign<TileviewComponent>();
	
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

