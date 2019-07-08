#include "sandbox.h"

Sandbox::Sandbox()
  : stella::core::Game(896, 504, "stella engine")
{
  this->add_system<stella::systems::PhysicsSystem>(m_registry, m_camera);
  //this->add_system<stella::systems::PlayerSystem>(m_display, m_sound_player);
  this->add_system<stella::systems::ScrollSystem>();
  this->add_system<stella::systems::TiledScrollSystem>(m_initial_width);
  this->add_system<stella::systems::TileSystem>(m_camera);
  this->add_system<stella::systems::MovementSystem>();

  m_script_api.set_function("e_get_player_id", [this]() {
      return m_player.entity;
  });
  m_script_api.run_script("./scripts/main.lua");
  m_script_api.run_function("load_game");
  
  m_sound_player.AddStream("dawn-pollen", "assets/audio/st-dawn_pollen.ogg");
  m_sound_player.Play("dawn-pollen", true);
}

Sandbox::~Sandbox()
{
  m_script_api.run_function("quit_game");
}

void Sandbox::update(const double dt)
{
  this->update_systems(dt);
  m_player.update();
  m_script_api.run_function("update_game", dt);
  m_script_api.run_function("render_game", dt);
  m_sound_player.Update();
}
