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
  this->scriptApi.vm.set_function("load_font", &Game::LoadFont, this);
  this->scriptApi.vm.set_function("e_create_entity", &Game::create_entity, this);
  this->scriptApi.vm.set_function("e_add_sprite_component", &Game::add_sprite_component, this);
  this->scriptApi.vm.set_function("e_add_dimension_component", &Game::add_dimension_component, this);
  this->scriptApi.vm.set_function("e_add_position_component", &Game::add_position_component, this);
  this->scriptApi.vm.set_function("e_add_animation_component", &Game::add_animation_component, this);
  this->scriptApi.vm.set_function("e_add_tileview_component", &Game::add_tileview_component, this);
  this->scriptApi.vm.set_function("e_add_movement_component", &Game::add_movement_component, this);
  this->scriptApi.vm.set_function("e_add_player_component", &Game::add_player_component, this);
  this->scriptApi.vm.set_function("e_add_body_component", &Game::add_body_component, this);
  this->scriptApi.vm.set_function("e_add_text_component", &Game::add_text_component, this);
  this->scriptApi.vm.set_function("e_add_particle_emitter_component", &Game::add_particle_emitter_component, this);
  this->scriptApi.RunScript("scripts/main.lua");
  this->scriptApi.RunLoad();

  // Load game entities
  this->load_game_info();

  //layer2 = entities.create();
  //layer2.assign<stella::components::LayerComponent>("ui", 1, "text");
  layer1 = entities.create();
  layer1.assign<stella::components::LayerComponent>("basic", 0, "basic");
  layer3 = entities.create();
  layer3.assign<stella::components::LayerComponent>("particles", 1, "bloom");
  

  // Add systems
  //systems.add<stella::systems::CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  systems.add<stella::systems::ParticleSystem>();
  systems.add<stella::systems::PhysicsSystem>();
  systems.add<stella::systems::SimpleMovementSystem>();
  systems.add<stella::systems::RenderingSystem>(this->Textures, this->Display);
  systems.add<stella::systems::TileviewSystem>((int)this->Display.GetWidth());
  systems.add<stella::systems::PlayerMovementSystem>((int)this->Display.GetWidth(), display);
  systems.add<stella::systems::TransformSystem>();
  //systems.add<stella::systems::TorchSystem>(player, entities);
  systems.add<stella::systems::AnimationSystem>();
  systems.add<stella::systems::GuiRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
  systems.configure();

  //std::function<void(double)> update_function = [=](double dt) {
    //this->update_systems(dt);
  //};
  //this->scriptApi.SetFunction<double, void>("update_game", update_function);
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::update_systems(const double &dt)
{
  //systems.update<stella::systems::CollisionSystem>(dt);
  systems.update<stella::systems::ParticleSystem>(dt);
  systems.update<stella::systems::PhysicsSystem>(dt);
  systems.update<stella::systems::SimpleMovementSystem>(dt);
  systems.update<stella::systems::RenderingSystem>(dt);
  systems.update<stella::systems::TileviewSystem>(dt);
  systems.update<stella::systems::PlayerMovementSystem>(dt);
  systems.update<stella::systems::TransformSystem>(dt);
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

void Game::load_game_info() {
  // Fonts
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

