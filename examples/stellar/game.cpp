#include "game.h"

Game::Game(stella::graphics::Display &display, stella::graphics::Shader *shader,
           const bool *keys) {
  systems.add<CollisionSystem>((int)display.GetWidth(),
                               (int)display.GetHeight());
  systems.add<MovementSystem>();
  systems.add<RenderSystem>((int)display.GetWidth(), (int)display.GetHeight(),
                            shader);
  systems.add<PlayerMovementSystem>((int)display.GetWidth());
  systems.add<LightingSystem>(shader);
  systems.add<AnimationSystem>();
  systems.add<TileviewSystem>((int)display.GetWidth());
  systems.add<ParallaxSystem>();
  systems.configure();

  // Background
  entityx::Entity sky = entities.create();
  entityx::Entity moon = entities.create();
  SkyTex = new stella::graphics::Texture("sky-tex",
                                         "assets/sprites/sky_background.png");
  MoonTex =
      new stella::graphics::Texture("moon-tex", "assets/sprites/moon_anim.png");

  sky.assign<TextureComponent>(720, 405, *SkyTex, 0);
  sky.assign<SpatialComponent>(720, 405);

  moon.assign<TextureComponent>(85, 85, *MoonTex, 0);
  moon.assign<SpatialComponent>(85, 85, 478, 78);

  add_animation(moon, "moon", {3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20);
  moon.assign<AnimationComponent>("moon");

  entityx::Entity mou1 = entities.create();
  Mou1 = new stella::graphics::Texture("mou1-tex",
                                       "assets/sprites/mountain1-bg.png");
  mou1.assign<TextureComponent>(720, 170, *Mou1, 0);
  mou1.assign<SpatialComponent>(720, 170, 0, 230);
  mou1.assign<ParallaxComponent>(-1.0f);
  mou1.assign<TileviewComponent>();
	entityx::Entity mou1a = entities.create();
	mou1a.assign<TextureComponent>(720, 170, *Mou1, 0);
	mou1a.assign<SpatialComponent>(720, 170, 720, 230);
	mou1a.assign<ParallaxComponent>(-1.0f);
	mou1a.assign<TileviewComponent>();

  entityx::Entity mou2 = entities.create();
  Mou2 = new stella::graphics::Texture("mou2-tex",
                                       "assets/sprites/mountain2-bg.png");
  mou2.assign<TextureComponent>(720, 190, *Mou2, 0);
  mou2.assign<SpatialComponent>(720, 190, 0, 215);
  mou2.assign<ParallaxComponent>(-3.0f);
  mou2.assign<TileviewComponent>();
	entityx::Entity mou2a = entities.create();
	mou2a.assign<TextureComponent>(720, 190, *Mou2, 0);
	mou2a.assign<SpatialComponent>(720, 190, 720, 215);
	mou2a.assign<ParallaxComponent>(-3.0f);
	mou2a.assign<TileviewComponent>();

  entityx::Entity mou3 = entities.create();
  Mou3 = new stella::graphics::Texture("mou3-tex",
                                       "assets/sprites/mountain3-bg.png");
  mou3.assign<TextureComponent>(720, 230, *Mou3, 0);
  mou3.assign<SpatialComponent>(720, 230, 0, 175);
  mou3.assign<ParallaxComponent>(-5.0f);
  mou3.assign<TileviewComponent>();
	entityx::Entity mou3a = entities.create();
	mou3a.assign<TextureComponent>(720, 239, *Mou3, 0);
	mou3a.assign<SpatialComponent>(720, 230, 720, 175);
	mou3a.assign<ParallaxComponent>(-5.0f);
	mou3a.assign<TileviewComponent>();

  // Player
  entityx::Entity player = entities.create();
  PlayerTex = new stella::graphics::Texture("guanaco-tex",
                                            "assets/sprites/guanaco-anim.png");
  // PlayerTex = new stella::graphics::Texture("guanaco-tex",
  // "assets/sprites/black_block.png");

  // player.assign<BodyComponent>(64, 64, 0, 0, false);
  // player.assign<TextureComponent>(64, 64, *PlayerTex, 0);
  // player.assign<SpatialComponent>(64, 64, 140, 250);
  player.assign<BodyComponent>(80, 60, 0, 0, false);
  player.assign<TextureComponent>(80, 60, *PlayerTex, 0);
  player.assign<SpatialComponent>(80, 60, 140, 250);
  player.assign<MovementComponent>(0.7f, 8.0f, 1.5f);
  player.assign<InputComponent>(keys);
  player.assign<LightComponent>(0, 1.0f);

  add_animation(player, "run", {0, 1, 2, 3, 4}, 5);
  player.assign<AnimationComponent>("run");

  // Terrain
  entityx::Entity block = entities.create();
  entityx::Entity over_block = entities.create();
  entityx::Entity over_block2 = entities.create();
  BlockTex =
      new stella::graphics::Texture("block-tex", "assets/sprites/block.png");
  OverBlockTex = new stella::graphics::Texture("details-tex",
                                               "assets/sprites/over_block.png");

  block.assign<BodyComponent>(720, 92, 0, 0, true);
  block.assign<TextureComponent>(720, 92, *BlockTex, 0);
  block.assign<SpatialComponent>(720, 92, 0, 313);

  over_block.assign<TextureComponent>(720, 12, *OverBlockTex, 0);
  over_block.assign<SpatialComponent>(720, 12, 0, 301);
  over_block.assign<MovementComponent>(-7.0f, 0.0f, 7.0f, false);
  over_block.assign<TileviewComponent>();

	over_block2.assign<TextureComponent>(720, 12, *OverBlockTex, 0);
	over_block2.assign<SpatialComponent>(720, 12, 720, 301);
	over_block2.assign<MovementComponent>(-7.0f, 0.0f, 7.0f, false);
	over_block2.assign<TileviewComponent>();
}

Game::~Game() {
  delete OverBlockTex;
  delete BlockTex;
  delete PlayerTex;
  delete Mou3;
  delete Mou2;
  delete Mou1;
  delete MoonTex;
  delete SkyTex;
}

void Game::add_animation(entityx::Entity &ent, std::string name,
                         std::vector<unsigned int> frames,
                         unsigned int framerate) {
  entityx::ComponentHandle<TextureComponent> tex =
      ent.component<TextureComponent>();
  if (tex) {
    tex->sprite->Animations.Add(name, frames, framerate);
  }
}

void Game::Update(entityx::TimeDelta dt) { systems.update_all(dt); }
