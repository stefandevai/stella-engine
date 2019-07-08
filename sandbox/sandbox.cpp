#include "sandbox.h"

Sandbox::Sandbox()
  : stella::core::Game(896, 504, "stella engine")
{
  this->add_system<stella::systems::PhysicsSystem>(m_registry, m_camera);
  this->add_system<stella::systems::PlayerSystem>(m_display, m_sound_player);
  this->add_system<stella::systems::ScrollSystem>();
  this->add_system<stella::systems::TiledScrollSystem>(m_initial_width);
  this->add_system<stella::systems::TileSystem>(m_camera);
  this->add_system<stella::systems::MovementSystem>();

  m_script_api.run_script("./scripts/main.lua");
  m_script_api.run_function("load_game");
}

Sandbox::~Sandbox()
{

}

void Sandbox::update(const double dt)
{
  this->update_systems(dt);
  m_script_api.run_function("update_game", dt);
  m_script_api.run_function("render_game", dt);
}
