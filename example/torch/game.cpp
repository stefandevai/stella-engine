#include "game.h" 

#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>
#include <functional>

#include <stella/components/game_components.h>
#include <stella/systems/game_systems.h>

Game::Game(stella::graphics::Display &display) : Display(display) {
  this->scriptApi.vm.set_function("load_texture", &Game::LoadTexture, this);
  this->scriptApi.vm.set_function("e_create_entity", &Game::create_entity, this);
  this->scriptApi.vm.set_function("e_add_sprite_component", &Game::add_sprite_component, this);
  this->scriptApi.vm.set_function("e_add_dimension_component", &Game::add_dimension_component, this);
  this->scriptApi.vm.set_function("e_add_position_component", &Game::add_position_component, this);
  this->scriptApi.vm.set_function("e_add_animation_component", &Game::add_animation_component, this);
  this->scriptApi.vm.set_function("e_add_tileview_component", &Game::add_tileview_component, this);
  this->scriptApi.vm.set_function("e_add_movement_component", &Game::add_movement_component, this);
  this->scriptApi.vm.set_function("e_add_player_component", &Game::add_player_component, this);
  this->scriptApi.vm.set_function("e_add_body_component", &Game::add_body_component, this);
  this->scriptApi.RunScript("scripts/main.lua");
  this->scriptApi.RunLoad();

  // Load game entities
  //this->load_background();
  //this->load_player(100, 200);
  //this->load_particles();
  //this->load_blocks();
  //this->load_text();

  // Add systems
  //systems.add<stella::systems::CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  //systems.add<stella::systems::ParticleSystem>();
  systems.add<stella::systems::PhysicsSystem>();
  systems.add<stella::systems::SimpleMovementSystem>();
  systems.add<stella::systems::SceneRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Textures, this->Display);
  systems.add<stella::systems::TileviewSystem>((int)this->Display.GetWidth());
  systems.add<stella::systems::PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  //systems.add<stella::systems::TransformSystem>();
  //systems.add<stella::systems::TorchSystem>(player, entities);
  systems.add<stella::systems::AnimationSystem>();
  //systems.add<stella::systems::GuiRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
  systems.configure();

  std::function<void(double)> update_function = [=](double dt) {
    this->update_systems(dt);
  };
  this->scriptApi.SetFunction<double, void>("update_game", update_function);
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::update_systems(const double &dt)
{
  //systems.update<stella::systems::CollisionSystem>(dt);
  //systems.update<stella::systems::ParticleSystem>(dt);
  systems.update<stella::systems::PhysicsSystem>(dt);
  systems.update<stella::systems::SimpleMovementSystem>(dt);
  systems.update<stella::systems::SceneRenderingSystem>(dt);
  systems.update<stella::systems::TileviewSystem>(dt);
  systems.update<stella::systems::PlayerMovementSystem>(dt);
  //systems.update<stella::systems::TransformSystem>(dt);
  //systems.update<stella::systems::TorchSystem>(dt);
  systems.update<stella::systems::AnimationSystem>(dt);
  //systems.update<stella::systems::GuiRenderingSystem>(dt);

  //if (this->FPSText && this->Display.GetFrame() % 30 == 0) {
    //std::stringstream fps_string("");
    //fps_string << std::setprecision(4) << 1/dt << " FPS";
    //auto text = this->FPSText.component<stella::components::TextComponent>();
    //text->Text = fps_string.str();
  //}
}


void Game::Update(ex::TimeDelta dt) { 
  this->scriptApi.RunUpdate(dt);
  this->scriptApi.RunRender(dt);
  this->update_systems(dt);

  //const unsigned int state = scriptApi.GetVariable<const unsigned int>("current_state");
  //switch(state) {
    //case GAME_LOADING:
      //this->scriptApi.RunLoad();
      ////this->lua["load"]();
      //break;
    //case GAME_LOADED:
      //break;
    //case GAME_NOT_LOADED:
      //break;
    //case MAIN_MENU:
      //break;
    //case GAME_LOOP:
      //this->scriptApi.RunFunction("loop");
      ////this->lua["loop"]();
      //break;
    //case GAME_PAUSED:
      //break;
    //case GAME_OVER:
      //break;
    //default:
      //break;
  //}
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
  this->LoadTexture("over_block", "assets/sprites/over_block.png");
  this->LoadTexture("tiles", "assets/sprites/tiles.png");

  // Terrain
  unsigned int nblock = this->Display.GetWidth() / 32 + 1;
  for (unsigned int i = 0; i < nblock; ++i) {
    ex::Entity block = entities.create();
    block.assign<stella::components::Body2DComponent>();
    block.assign<stella::components::PositionComponent>(32.f * (float)i, this->Display.GetHeight() - 64);
    block.assign<stella::components::DimensionComponent>(32.f, 32.f);
    block.assign<stella::components::SpriteComponent>("tiles", glm::vec2(32, 32));

    ex::Entity block2 = entities.create();
    block2.assign<stella::components::Body2DComponent>();
    block2.assign<stella::components::PositionComponent>(32.f * (float)i, this->Display.GetHeight() - 32);
    block2.assign<stella::components::DimensionComponent>(32.f, 32.f);
    block2.assign<stella::components::SpriteComponent>("tiles", glm::vec2(32, 32), 0);
  }

  // Over terrain decoration
  auto over_terrain = entities.create();
  over_terrain.assign<stella::components::PositionComponent>(0, this->Display.GetHeight() - 76);
  over_terrain.assign<stella::components::SpriteComponent>("over_block");
  over_terrain.assign<stella::components::DimensionComponent>(720.f, 12.f);
  over_terrain.assign<stella::components::MovementComponent>(glm::vec2(-200.f, 0.f), false, true);
  over_terrain.assign<stella::components::TileviewComponent>();

  auto over_terrain2 = entities.create();
  over_terrain2.assign<stella::components::PositionComponent>(720, this->Display.GetHeight() - 76);
  over_terrain2.assign<stella::components::SpriteComponent>("over_block");
  over_terrain2.assign<stella::components::DimensionComponent>(720.f, 12.f);
  over_terrain2.assign<stella::components::MovementComponent>(glm::vec2(-200.f, 0.f), false, true);
  over_terrain2.assign<stella::components::TileviewComponent>();
}

void Game::load_text() {
  // Fonts
  this->LoadFont("font-cursive", "assets/sprites/cursive.png");

	auto title_text = entities.create();
  title_text.assign<stella::components::PositionComponent>(30.f, 30.f);
  title_text.assign<stella::components::DimensionComponent>(9.f, 9.f);
	title_text.assign<stella::components::TextComponent>("- TORCH -", "font-cursive", true);

  const unsigned char* renderer = this->Display.GetGlRenderer();
  std::stringstream renderer_string("");
  renderer_string << renderer;
  auto renderer_info = entities.create();
  renderer_info.assign<stella::components::PositionComponent>(30.f, 60.f);
  renderer_info.assign<stella::components::DimensionComponent>(9.f, 9.f);
  renderer_info.assign<stella::components::TextComponent>(renderer_string.str(), "font-cursive", true);

  const unsigned char* version = this->Display.GetGlVersion();
  std::stringstream version_string("");
  version_string << "OpenGL " << version;
  auto opengl_info = entities.create();
  opengl_info.assign<stella::components::PositionComponent>(30.f, 75.f);
  opengl_info.assign<stella::components::DimensionComponent>(9.f, 9.f);
  opengl_info.assign<stella::components::TextComponent>(version_string.str(), "font-cursive", true);

  this->FPSText = entities.create();
  this->FPSText.assign<stella::components::PositionComponent>(30.f, 90.f);
  this->FPSText.assign<stella::components::DimensionComponent>(9.f, 9.f);
  this->FPSText.assign<stella::components::TextComponent>("", "font-cursive");
}

void Game::load_background() {
  //this->LoadTexture("sky", "assets/sprites/sky_background.png");
  this->LoadTexture("moon2", "assets/sprites/moon_anim.png");
  //this->LoadTexture("mountain1", "assets/sprites/mountain1-bg.png");
  //this->LoadTexture("mountain2", "assets/sprites/mountain2-bg.png");
  //this->LoadTexture("mountain3", "assets/sprites/mountain3-bg.png");

  // Background
  //auto sky = entities.create();
  //sky.assign<stella::components::SpriteComponent>("sky");
  //sky.assign<stella::components::DimensionComponent>(720.f, 405.f);
  //sky.assign<stella::components::PositionComponent>(0.f, 0.f);

  auto moon = entities.create();
  moon.assign<stella::components::SpriteComponent>("moon2", glm::vec2(85, 85));
  std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> moon_anims;
  moon_anims.emplace_back("moon-anim", std::vector<unsigned int>{3, 0, 4, 2, 1, 4, 3, 0, 2, 4, 3}, 20);
  moon.assign<stella::components::AnimationsComponent>(moon_anims, glm::vec2(85, 85));
  moon.assign<stella::components::DimensionComponent>(85.f, 85.f);
  moon.assign<stella::components::PositionComponent>(78.f, 78.f);

	// Background mountains
  //auto mou1 = entities.create();
  //mou1.assign<stella::components::SpriteComponent>("mountain1");
  //mou1.assign<stella::components::DimensionComponent>(720.f, 170.f);
  //mou1.assign<stella::components::PositionComponent>(0.f, 230.f);
  //mou1.assign<stella::components::MovementComponent>(glm::vec2(-3.f, 0.f), false, true);
  //mou1.assign<stella::components::TileviewComponent>();

  //auto mou1a = entities.create();
  //mou1a.assign<stella::components::SpriteComponent>("mountain1");
  //mou1a.assign<stella::components::DimensionComponent>(720.f, 170.f);
  //mou1a.assign<stella::components::PositionComponent>(720.f, 230.f);
  //mou1a.assign<stella::components::MovementComponent>(glm::vec2(-3.f, 0.f), false, true);
  //mou1a.assign<stella::components::TileviewComponent>();

  //auto mou2 = entities.create();
  //mou2.assign<stella::components::SpriteComponent>("mountain2");
  //mou2.assign<stella::components::DimensionComponent>(720.f, 190.f);
  //mou2.assign<stella::components::PositionComponent>(0.f, 215.f);
  //mou2.assign<stella::components::MovementComponent>(glm::vec2(-9.f, 0.f), false, true);
  //mou2.assign<stella::components::TileviewComponent>();

  //auto mou2a = entities.create();
  //mou2a.assign<stella::components::SpriteComponent>("mountain2");
  //mou2a.assign<stella::components::DimensionComponent>(720.f, 190.f);
  //mou2a.assign<stella::components::PositionComponent>(720.f, 215.f);
  //mou2a.assign<stella::components::MovementComponent>(glm::vec2(-9.f, 0.f), false, true);
  //mou2a.assign<stella::components::TileviewComponent>();

  //auto mou3 = entities.create();
  //mou3.assign<stella::components::SpriteComponent>("mountain3");
  //mou3.assign<stella::components::DimensionComponent>(720.f, 230.f);
  //mou3.assign<stella::components::PositionComponent>(0.f, 175.f);
  //mou3.assign<stella::components::MovementComponent>(glm::vec2(-15.f, 0.f), false, true);
  //mou3.assign<stella::components::TileviewComponent>();

  //auto mou3a = entities.create();
  //mou3a.assign<stella::components::SpriteComponent>("mountain3");
  //mou3a.assign<stella::components::DimensionComponent>(720.f, 230.f);
  //mou3a.assign<stella::components::PositionComponent>(720.f, 175.f);
  //mou3a.assign<stella::components::MovementComponent>(glm::vec2(-15.f, 0.f), false, true);
  //mou3a.assign<stella::components::TileviewComponent>();
}

void Game::load_particles() {
  this->LoadTexture("fire-particle", "assets/sprites/fire-particle.png");
  auto torch = entities.create();
  torch.assign<stella::components::ParticleEmitter>(stella::components::ParticleEmitter::Type::FIRE_EMITTER, 10);
  torch.assign<stella::components::PositionComponent>(350.f, 290.f);
  torch.assign<stella::components::DimensionComponent>(16.f, 16.f);
  torch.assign<stella::components::TorchComponent>();

  this->LoadTexture("snowflake", "assets/snowflakes/flake2-small.png");
  auto snow = entities.create();
  snow.assign<stella::components::ParticleEmitter>(stella::components::ParticleEmitter::Type::SNOW_EMITTER, 10);
  snow.assign<stella::components::PositionComponent>(0.0f, -64.f);
  snow.assign<stella::components::DimensionComponent>(32.f, 32.f);
}

void Game::load_player(int x, int y) {
  this->LoadTexture("guanaco", "assets/sprites/guanaco-anim.png");

  auto player = entities.create();
  player.assign<stella::components::SpriteComponent>("guanaco", glm::vec2(80.f, 60.f));
  std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> guanaco_anims;
  guanaco_anims.emplace_back("running", std::vector<unsigned int>{0, 1, 2, 3, 4}, 5);
  player.assign<stella::components::AnimationsComponent>(guanaco_anims, glm::vec2(80, 60));
  player.assign<stella::components::Body2DComponent>(std::vector<double>(500.f, 500.f));
  player.assign<stella::components::PositionComponent>((float)x, (float)y);
  player.assign<stella::components::DimensionComponent>(80.f, 60.f);
  player.assign<stella::components::MovementComponent>(glm::vec2(450.f, 400.f));
  player.assign<stella::components::PlayerComponent>();
}

