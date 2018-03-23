#include "game.h"

#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>

Game::Game(stella::graphics::Display &display) : Display(display) {
  systems.add<CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  systems.add<MovementSystem>();
  systems.add<FontRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
	systems.add<RenderSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Textures);
  systems.add<PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  systems.add<TileviewSystem>((int)this->Display.GetWidth());
  systems.add<ParallaxSystem>();
  systems.add<ParticleSystem>();
  systems.configure();

  // Textures
  this->LoadTexture("sky", "assets/sprites/sky_background.png");
  this->LoadTexture("moon", "assets/sprites/moon_anim.png");
  this->LoadTexture("mountain1", "assets/sprites/mountain1-bg.png");
  this->LoadTexture("mountain2", "assets/sprites/mountain2-bg.png");
  this->LoadTexture("mountain3", "assets/sprites/mountain3-bg.png");
  this->LoadTexture("block", "assets/sprites/block.png");
  this->LoadTexture("over_block", "assets/sprites/over_block.png");
  this->LoadTexture("guanaco", "assets/sprites/guanaco-anim.png");
	this->LoadTexture("fire-particle", "assets/sprites/fire_particle.png");

  // Fonts
  this->LoadFont("font", "assets/sprites/font.png");
  this->LoadFont("font-outlined", "assets/sprites/font_outlined.png");
  this->LoadFont("font-cursive", "assets/sprites/cursive.png");

	this->load_background();
	this->load_blocks();
	this->load_player(150, 253);
	this->load_foreground();
	this->load_text();

	auto fire = entities.create();
  //fire.assign<SpriteComponent>("fire-particle");
	fire.assign<SpatialComponent>(16, 16, 300, 200);
	fire.assign<ParticleGenerator>();
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::Update(entityx::TimeDelta dt) { 
  systems.update<CollisionSystem>(dt);
  systems.update<MovementSystem>(dt);
	systems.update<RenderSystem>(dt);
  systems.update<PlayerMovementSystem>(dt);
  systems.update<TileviewSystem>(dt);
  systems.update<ParallaxSystem>(dt);
  systems.update<FontRenderingSystem>(dt);
  systems.update<ParticleSystem>(dt);

	if (this->Display.GetFrame() % 30 == 0) {
		std::stringstream fps_string("");
		fps_string << std::setprecision(4) << 1/dt << " FPS";
		auto text = this->FPSText.component<TextComponent>();
		text->Text = fps_string.str();
	}
}

void Game::LoadTexture(std::string tex_name, const char *tex_path) {
	stella::graphics::Texture *texture = new stella::graphics::Texture(tex_name, tex_path);
	std::pair<std::string, stella::graphics::Texture*> cached_texture(tex_name, texture);
	this->Textures.insert(cached_texture);
}

void Game::LoadFont(std::string font_name, const char *font_path) {
	stella::graphics::Texture *texture = new stella::graphics::Texture(font_name, font_path);
	std::pair<std::string, stella::graphics::Texture*> font(font_name, texture);
	this->Fonts.insert(font);
}

void Game::load_background() {
  // Background
  entityx::Entity sky = entities.create();
 	sky.assign<SpriteComponent>("sky");
  sky.assign<SpatialComponent>(720, 405);

  entityx::Entity moon = entities.create();
  moon.assign<SpriteComponent>("moon");
	std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> moon_anims;
	moon_anims.emplace_back("moon-anim", std::vector<unsigned int>{3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20);
	moon.assign<AnimationsComponent>(moon_anims);
  moon.assign<SpatialComponent>(85, 85, 478, 78);

	// Background mountains
  entityx::Entity mou1 = entities.create();
  mou1.assign<SpriteComponent>("mountain1");
  mou1.assign<SpatialComponent>(720, 170, 0, 230);
  mou1.assign<ParallaxComponent>(-1.0f);
  mou1.assign<TileviewComponent>();

  entityx::Entity mou1a = entities.create();
  mou1a.assign<SpriteComponent>("mountain1");
  mou1a.assign<SpatialComponent>(720, 170, 720, 230);
  mou1a.assign<ParallaxComponent>(-1.0f);
  mou1a.assign<TileviewComponent>();

  entityx::Entity mou2 = entities.create();
  mou2.assign<SpriteComponent>("mountain2");
  mou2.assign<SpatialComponent>(720, 190, 0, 215);
  mou2.assign<ParallaxComponent>(-3.0f);
  mou2.assign<TileviewComponent>();

  entityx::Entity mou2a = entities.create();
  mou2a.assign<SpriteComponent>("mountain2");
  mou2a.assign<SpatialComponent>(720, 190, 720, 215);
  mou2a.assign<ParallaxComponent>(-3.0f);
  mou2a.assign<TileviewComponent>();

  entityx::Entity mou3 = entities.create();
  mou3.assign<SpriteComponent>("mountain3");
  mou3.assign<SpatialComponent>(720, 230, 0, 175);
  mou3.assign<ParallaxComponent>(-5.0f);
  mou3.assign<TileviewComponent>();

  entityx::Entity mou3a = entities.create();
  mou3a.assign<SpriteComponent>("mountain3");
  mou3a.assign<SpatialComponent>(720, 230, 720, 175);
  mou3a.assign<ParallaxComponent>(-5.0f);
  mou3a.assign<TileviewComponent>();
}

void Game::load_player(int x, int y) {
  // Player
  entityx::Entity player = entities.create();
  player.assign<SpriteComponent>("guanaco");
	std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> guanaco_anims;
	guanaco_anims.emplace_back("running", std::vector<unsigned int>{0, 1, 2, 3, 4}, 5);
	player.assign<AnimationsComponent>(guanaco_anims);
  player.assign<BodyComponent>(80, 60, 0, 0, false);
  player.assign<SpatialComponent>(80, 60, x, y);
  player.assign<MovementComponent>(0.7f, 8.0f, 1.5f);
  player.assign<InputComponent>();
  player.assign<LightComponent>(0, 1.0f);
}

void Game::load_blocks() {
  // Terrain
  entityx::Entity block = entities.create();
  block.assign<BodyComponent>(720, 92, 0, 0, true);
  block.assign<SpatialComponent>(720, 92, 0, 313);
  block.assign<SpriteComponent>("block");
}

void Game::load_foreground() {
  entityx::Entity over_block = entities.create();
  over_block.assign<SpatialComponent>(720, 12, 0, 301);
  over_block.assign<ParallaxComponent>(-7.0f);
  over_block.assign<TileviewComponent>();
  over_block.assign<SpriteComponent>("over_block");

  entityx::Entity over_block2 = entities.create();
  over_block2.assign<SpatialComponent>(720, 12, 720, 301);
  over_block2.assign<ParallaxComponent>(-7.0f);
  over_block2.assign<TileviewComponent>();
  over_block2.assign<SpriteComponent>("over_block");
}

void Game::load_text() {
	auto title_text = entities.create();
	title_text.assign<SpatialComponent>(9, 9, 30, 30);
	title_text.assign<TextComponent>("Stella Engine", "font-cursive", true);

	this->FPSText = entities.create();
	this->FPSText.assign<SpatialComponent>(9, 9, 30, 45);
	this->FPSText.assign<TextComponent>("", "font-cursive");
}

