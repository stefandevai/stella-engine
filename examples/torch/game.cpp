#include "game.h" 

#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>

#include "components/game_components.h"
#include "systems/game_systems.h"

Game::Game(stella::graphics::Display &display) : Display(display) {
  // Textures
  this->LoadTexture("player", "assets/sprites/player.png");
  //this->LoadTexture("player", "assets/sprites/bug-64.png");
  this->LoadTexture("ground", "assets/sprites/ground.png");
  this->LoadTexture("tiles", "assets/sprites/tiles.png");
	this->LoadTexture("fire-particle", "assets/sprites/fire-particle.png");
	this->LoadTexture("snowflake", "assets/snowflakes/flake2-small.png");

  this->LoadTexture("sky", "assets/sprites/sky_background.png");
  this->LoadTexture("moon", "assets/sprites/moon_anim.png");
  this->LoadTexture("mountain1", "assets/sprites/mountain1-bg.png");
  this->LoadTexture("mountain2", "assets/sprites/mountain2-bg.png");
  this->LoadTexture("mountain3", "assets/sprites/mountain3-bg.png");
  this->LoadTexture("block", "assets/sprites/block.png");
  this->LoadTexture("over_block", "assets/sprites/over_block.png");
  this->LoadTexture("guanaco", "assets/sprites/guanaco-anim.png");

  // Fonts
  this->LoadFont("font-cursive", "assets/sprites/cursive.png");

  // Entities
  this->load_background();
  this->load_player(100, 200);

  auto torch = entities.create();
  torch.assign<ParticleEmitter>(ParticleEmitter::Type::FIRE_EMITTER, 10);
  torch.assign<PositionComponent>(350.f, 290.f);
  torch.assign<DimensionComponent>(16.f, 16.f);
  torch.assign<TorchComponent>();

  //auto snow = entities.create();
  //snow.assign<ParticleEmitter>(ParticleEmitter::Type::SNOW_EMITTER, 10);
  //snow.assign<PositionComponent>(0.0f, -64.f);
  //snow.assign<DimensionComponent>(32.f, 32.f);

  this->load_blocks();
  this->load_text();

  //// Systems
  //systems.add<CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  systems.add<ParticleSystem>();
  systems.add<PhysicsSystem>();
  systems.add<SimpleMovementSystem>();
  systems.add<SceneRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Textures, this->Display);
  systems.add<TileviewSystem>((int)this->Display.GetWidth());
  //systems.add<ParallaxSystem>();
  systems.add<PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  systems.add<TransformSystem>();
  //systems.add<TorchSystem>(player, entities);
  systems.add<AnimationSystem>();
  systems.add<GuiRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
  systems.configure();
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::Update(ex::TimeDelta dt) { 
  //systems.update<CollisionSystem>(dt);
  systems.update<ParticleSystem>(dt);
  systems.update<PhysicsSystem>(dt);
  systems.update<SimpleMovementSystem>(dt);
  systems.update<SceneRenderingSystem>(dt);
  systems.update<TileviewSystem>(dt);
  systems.update<PlayerMovementSystem>(dt);
  systems.update<TransformSystem>(dt);
  //systems.update<TorchSystem>(dt);
  systems.update<AnimationSystem>(dt);
  systems.update<GuiRenderingSystem>(dt);

	if (this->FPSText && this->Display.GetFrame() % 30 == 0) {
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

void Game::load_blocks() {
  // Terrain
  unsigned int nblock = this->Display.GetWidth() / 32 + 1;
  for (unsigned int i = 0; i < nblock; ++i) {
    ex::Entity block = entities.create();
    block.assign<Body2DComponent>();
    block.assign<PositionComponent>(32.f * (float)i, this->Display.GetHeight() - 64);
    block.assign<DimensionComponent>(32.f, 32.f);
    block.assign<SpriteComponent>("tiles", glm::vec2(32, 32));

    ex::Entity block2 = entities.create();
    block2.assign<Body2DComponent>();
    block2.assign<PositionComponent>(32.f * (float)i, this->Display.GetHeight() - 32);
    block2.assign<DimensionComponent>(32.f, 32.f);
    block2.assign<SpriteComponent>("tiles", glm::vec2(32, 32), 0);
  }
}

void Game::load_text() {
	auto title_text = entities.create();
  title_text.assign<PositionComponent>(30.f, 30.f);
  title_text.assign<DimensionComponent>(9.f, 9.f);
	title_text.assign<TextComponent>("- TORCH -", "font-cursive", true);

  const unsigned char* renderer = this->Display.GetGlRenderer();
  std::stringstream renderer_string("");
  renderer_string << renderer;
  auto renderer_info = entities.create();
  renderer_info.assign<PositionComponent>(30.f, 60.f);
  renderer_info.assign<DimensionComponent>(9.f, 9.f);
  renderer_info.assign<TextComponent>(renderer_string.str(), "font-cursive", true);

  const unsigned char* version = this->Display.GetGlVersion();
  std::stringstream version_string("");
  version_string << "OpenGL " << version;
  auto opengl_info = entities.create();
  opengl_info.assign<PositionComponent>(30.f, 75.f);
  opengl_info.assign<DimensionComponent>(9.f, 9.f);
  opengl_info.assign<TextComponent>(version_string.str(), "font-cursive", true);

  this->FPSText = entities.create();
  this->FPSText.assign<PositionComponent>(30.f, 90.f);
  this->FPSText.assign<DimensionComponent>(9.f, 9.f);
  this->FPSText.assign<TextComponent>("", "font-cursive");
}

void Game::load_background() {
  // Background
  auto sky = entities.create();
  sky.assign<SpriteComponent>("sky");
	sky.assign<DimensionComponent>(720.f, 405.f);
	sky.assign<PositionComponent>(0.f, 0.f);

  auto moon = entities.create();
  moon.assign<SpriteComponent>("moon", glm::vec2(85, 85));
  std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> moon_anims;
  moon_anims.emplace_back("moon-anim", std::vector<unsigned int>{3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20);
  moon.assign<AnimationsComponent>(moon_anims, glm::vec2(85, 85));
  moon.assign<DimensionComponent>(85.f, 85.f);
  moon.assign<PositionComponent>(478.f, 78.f);

	// Background mountains
  auto mou1 = entities.create();
  mou1.assign<SpriteComponent>("mountain1");
  mou1.assign<DimensionComponent>(720.f, 170.f);
  mou1.assign<PositionComponent>(0.f, 230.f);
  mou1.assign<MovementComponent>(glm::vec2(-50.f, 0.f), false, true);
  mou1.assign<TileviewComponent>();

  auto mou1a = entities.create();
  mou1a.assign<SpriteComponent>("mountain1");
  mou1a.assign<DimensionComponent>(720.f, 170.f);
  mou1a.assign<PositionComponent>(720.f, 230.f);
  mou1a.assign<MovementComponent>(glm::vec2(-50.f, 0.f), false, true);
  mou1a.assign<TileviewComponent>();

  auto mou2 = entities.create();
  mou2.assign<SpriteComponent>("mountain2");
  mou2.assign<DimensionComponent>(720.f, 190.f);
  mou2.assign<PositionComponent>(0.f, 215.f);
  mou2.assign<MovementComponent>(glm::vec2(-100.f, 0.f), false, true);
  mou2.assign<TileviewComponent>();

  auto mou2a = entities.create();
  mou2a.assign<SpriteComponent>("mountain2");
  mou2a.assign<DimensionComponent>(720.f, 190.f);
  mou2a.assign<PositionComponent>(720.f, 215.f);
  mou2a.assign<MovementComponent>(glm::vec2(-100.f, 0.f), false, true);
  mou2a.assign<TileviewComponent>();

  auto mou3 = entities.create();
  mou3.assign<SpriteComponent>("mountain3");
  mou3.assign<DimensionComponent>(720.f, 230.f);
  mou3.assign<PositionComponent>(0.f, 175.f);
  mou3.assign<MovementComponent>(glm::vec2(-180.f, 0.f), false, true);
  mou3.assign<TileviewComponent>();

  auto mou3a = entities.create();
  mou3a.assign<SpriteComponent>("mountain3");
  mou3a.assign<DimensionComponent>(720.f, 230.f);
  mou3a.assign<PositionComponent>(720.f, 175.f);
  mou3a.assign<MovementComponent>(glm::vec2(-180.f, 0.f), false, true);
  mou3a.assign<TileviewComponent>();
}

void Game::load_player(int x, int y) {
  auto player = entities.create();
  player.assign<SpriteComponent>("guanaco", glm::vec2(80.f, 60.f));
  std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> guanaco_anims;
  guanaco_anims.emplace_back("running", std::vector<unsigned int>{0, 1, 2, 3, 4}, 5);
  player.assign<AnimationsComponent>(guanaco_anims, glm::vec2(80, 60));
  player.assign<Body2DComponent>(std::vector<double>(500.f, 500.f));
  player.assign<PositionComponent>((float)x, (float)y);
  player.assign<DimensionComponent>(80.f, 60.f);
  player.assign<MovementComponent>(glm::vec2(450.f, 400.f));
  player.assign<PlayerComponent>();
}

