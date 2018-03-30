#include "game.h" 
#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>

Game::Game(stella::graphics::Display &display) : Display(display) {
  // Textures
  this->LoadTexture("player", "assets/sprites/player.png");
  this->LoadTexture("ground", "assets/sprites/ground.png");
	this->LoadTexture("fire-particle", "assets/sprites/fire-particle.png");
	this->LoadTexture("snowflake", "assets/sprites/snowflake.png");

  // Fonts
  this->LoadFont("font-cursive", "assets/sprites/cursive.png");

  // Entities
  // Player
  auto player = entities.create();
  player.assign<SpriteComponent>("player", glm::vec2(32, 32));
  player.assign<BodyComponent>(32, 32, 0, 0, false);
  player.assign<SpatialComponent>(32, 32, 100, 336);
  player.assign<MovementComponent>(0.7f, 8.0f, 1.5f);
  player.assign<InputComponent>();
  player.assign<PlayerComponent>();

  auto torch = entities.create();
  torch.assign<ParticleEmitter>(ParticleEmitter::Type::FIRE_EMITTER, 10);
  torch.assign<SpatialComponent>(16, 16, 350, 290);
  torch.assign<TorchComponent>();

  auto snow = entities.create();
  snow.assign<ParticleEmitter>(ParticleEmitter::Type::SNOW_EMITTER, 10);
  snow.assign<SpatialComponent>(16, 16, 384, -64);

  this->load_blocks();
  this->load_text();

  // Systems
  systems.add<CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  systems.add<MovementSystem>();
  systems.add<ParticleSystem>();
  systems.add<RenderSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Textures, this->Display);
  systems.add<TransformSystem>();
  systems.add<PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  systems.add<TorchSystem>(player);
  systems.add<AnimationSystem>();
  systems.add<FontRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
  systems.configure();
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::Update(entityx::TimeDelta dt) { 
  systems.update<CollisionSystem>(dt);
  systems.update<MovementSystem>(dt);
  systems.update<ParticleSystem>(dt);
  systems.update<RenderSystem>(dt);
  systems.update<TransformSystem>(dt);
  systems.update<PlayerMovementSystem>(dt);
  systems.update<TorchSystem>(dt);
  systems.update<AnimationSystem>(dt);
  systems.update<FontRenderingSystem>(dt);

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

void Game::load_player(int x, int y) {
  //// Player
  //entityx::Entity player = entities.create();
  //player.assign<SpriteComponent>("player", glm::vec2(32, 32));
  //player.assign<BodyComponent>(32, 32, 0, 0, false);
  //player.assign<SpatialComponent>(32, 32, x, y);
  //player.assign<MovementComponent>(0.7f, 8.0f, 1.5f);
  //player.assign<InputComponent>();
}
 
void Game::load_blocks() {
  // Terrain
  entityx::Entity block = entities.create();
  block.assign<BodyComponent>(768, 64, 0, 0, true);
  block.assign<SpatialComponent>(768, 64, 0, 368);
  block.assign<SpriteComponent>("ground");
}

void Game::load_text() {
	auto title_text = entities.create();
	title_text.assign<SpatialComponent>(9, 9, 30, 30);
	title_text.assign<TextComponent>("- TORCH -", "font-cursive", true);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	std::stringstream renderer_string("");
	renderer_string << renderer;
	auto renderer_info = entities.create();
	renderer_info.assign<SpatialComponent>(9, 9, 30, 60);
	renderer_info.assign<TextComponent>(renderer_string.str(), "font-cursive", true);

	const GLubyte* version = glGetString(GL_VERSION);
	std::stringstream version_string("");
	version_string << "OpenGL " << version;
	auto opengl_info = entities.create();
	opengl_info.assign<SpatialComponent>(9, 9, 30, 75);
	opengl_info.assign<TextComponent>(version_string.str(), "font-cursive", true);

	this->FPSText = entities.create();
	this->FPSText.assign<SpatialComponent>(9, 9, 30, 90);
	this->FPSText.assign<TextComponent>("", "font-cursive");
}

