#include "game.h" 

#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>

#include "components/game_components.h"
#include "systems/game_systems.h"

Game::Game(stella::graphics::Display &display) : Display(display) {
  // Textures
  //this->LoadTexture("player", "assets/sprites/player.png");
  this->LoadTexture("player", "assets/sprites/bug-64.png");
  this->LoadTexture("ground", "assets/sprites/ground.png");
  this->LoadTexture("tiles", "assets/sprites/tiles.png");
	this->LoadTexture("fire-particle", "assets/sprites/fire-particle.png");
	this->LoadTexture("snowflake", "assets/snowflakes/flake2-small.png");

  // Fonts
  this->LoadFont("font-cursive", "assets/sprites/cursive.png");

  // Entities
  // Player
  auto player = entities.create();
  player.assign<SpriteComponent>("player", glm::vec2(64.f, 64.f));
  player.assign<Body2DComponent>();
  player.assign<PositionComponent>(100.f, 336.f);
  player.assign<DimensionComponent>(32.f, 32.f);
  player.assign<MovementComponent>(glm::vec2(250.f, 400.f));
  player.assign<PlayerComponent>();

  auto torch = entities.create();
  torch.assign<ParticleEmitter>(ParticleEmitter::Type::FIRE_EMITTER, 10);
  torch.assign<PositionComponent>(350.f, 290.f);
  torch.assign<DimensionComponent>(16.f, 16.f);
  torch.assign<TorchComponent>();

  auto snow = entities.create();
  snow.assign<ParticleEmitter>(ParticleEmitter::Type::SNOW_EMITTER, 10);
  snow.assign<PositionComponent>(0.0f, -64.f);
  snow.assign<DimensionComponent>(32.f, 32.f);

  this->load_blocks();
  this->load_text();

  //// Systems
  //systems.add<CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  systems.add<PhysicsSystem>();
  systems.add<ParticleSystem>();
  systems.add<PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  systems.add<SceneRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Textures, this->Display);
  systems.add<TransformSystem>();
  systems.add<TorchSystem>(player, entities);
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
  systems.update<PhysicsSystem>(dt);
  systems.update<ParticleSystem>(dt);
  systems.update<PlayerMovementSystem>(dt);
  systems.update<SceneRenderingSystem>(dt);
  systems.update<TransformSystem>(dt);
  systems.update<TorchSystem>(dt);
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
  unsigned int nblock = this->Display.GetWidth() / 32;
  //unsigned int nblock = 4;
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

  ex::Entity block = entities.create();
  block.assign<Body2DComponent>();
  //block.assign<PositionComponent>(32.f * 6.f, this->Display.GetHeight() - 96);
  block.assign<PositionComponent>(32.f * 0.f, this->Display.GetHeight() - 96);
  block.assign<DimensionComponent>(32.f, 32.f);
  block.assign<SpriteComponent>("tiles", glm::vec2(32, 32));

  ex::Entity block2 = entities.create();
  block2.assign<Body2DComponent>();
  block2.assign<PositionComponent>(32.f * 6.f, this->Display.GetHeight() - 96);
  block2.assign<DimensionComponent>(32.f, 32.f);
  block2.assign<SpriteComponent>("tiles", glm::vec2(32, 32), 0);

  ex::Entity block3 = entities.create();
  block3.assign<Body2DComponent>();
  block3.assign<PositionComponent>(32.f * 8.f, this->Display.GetHeight() - 128);
  block3.assign<DimensionComponent>(32.f, 32.f);
  block3.assign<SpriteComponent>("tiles", glm::vec2(32, 32), 0);
}

void Game::load_text() {
	auto title_text = entities.create();
  title_text.assign<PositionComponent>(30.f, 30.f);
  title_text.assign<DimensionComponent>(9.f, 9.f);
	title_text.assign<TextComponent>("- TORCH -", "font-cursive", true);

  //const unsigned char* renderer = glGetString(GL_RENDERER);
  //std::stringstream renderer_string("");
  //renderer_string << renderer;
  //auto renderer_info = entities.create();
  //renderer_info.assign<PositionComponent>(30.f, 60.f);
  //renderer_info.assign<DimensionComponent>(9.f, 9.f);
  //renderer_info.assign<TextComponent>(renderer_string.str(), "font-cursive", true);

  //const unsigned char* version = glGetString(GL_VERSION);
  //std::stringstream version_string("");
  //version_string << "OpenGL " << version;
  //auto opengl_info = entities.create();
  //opengl_info.assign<PositionComponent>(30.f, 75.f);
  //opengl_info.assign<DimensionComponent>(9.f, 9.f);
  //opengl_info.assign<TextComponent>(version_string.str(), "font-cursive", true);

  this->FPSText = entities.create();
  this->FPSText.assign<PositionComponent>(30.f, 90.f);
  this->FPSText.assign<DimensionComponent>(9.f, 9.f);
  this->FPSText.assign<TextComponent>("", "font-cursive");
}

