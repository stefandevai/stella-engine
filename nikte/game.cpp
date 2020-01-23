#include "game.h"

namespace nikte
{
Game::Game()
  : stella::core::Game(896, 504, "Nikte")
{
  this->add_system<stella::systems::PhysicsSystem>(m_tile_map, m_registry);
  this->add_system<stella::systems::TileSystem>(m_tile_map, m_camera, m_registry);
  this->add_system<stella::systems::TextSystem>(m_registry, m_fonts);
  this->add_system<stella::systems::SpeechSystem>();
  this->add_system<stella::systems::NPC>(m_player.entity);

  m_tile_map.create_tile_entities(0, m_display.GetWidth(), 0, m_display.GetHeight());

  m_script_api.set_function("e_get_player_id", [this]() {
      return m_player.entity;
  });
  
  m_script_api.run_script("./scripts/main.lua");
  m_script_api.run_function("load_game");

  m_script_api.run_script("./scripts/npcs/test.lua");
  auto func = m_script_api.get_variable<std::function<std::string(std::string)>>("talk");
  std::string res = func("hi");
  std::cout << res << '\n';

  this->update_systems(0.0);
}

Game::~Game()
{
  m_script_api.run_function("quit_game");
}

void Game::update(const double dt)
{
  this->update_systems(dt);

  m_player.update();
  m_script_api.run_function("update_game", dt);
  m_script_api.run_function("render_game", dt);
}
}
