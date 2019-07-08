#include "sandbox.h"

Sandbox::Sandbox()
  : stella::core::Game(896, 504, "stella engine")
{
  auto tex = m_textures.load("moon", "assets/sprites/moon_anim.png");
  auto entity = m_registry.create();
  m_registry.assign<stella::components::SpriteComponent>(entity, "moon");
  m_registry.assign<stella::components::PositionComponent>(entity, 32, 32);
  m_registry.assign<stella::components::DimensionComponent>(entity, 85, 85);

  auto physics_system = std::make_shared<stella::systems::PhysicsSystem>(m_registry, m_camera);
  auto player_system = std::make_shared<stella::systems::PlayerSystem>(m_display, m_sound_player);
  auto scroll_system = std::make_shared<stella::systems::ScrollSystem>();
  auto tiled_scroll_system = std::make_shared<stella::systems::TiledScrollSystem>((int)m_display.GetWidth());
  auto tile_system = std::make_shared<stella::systems::TileSystem>(m_camera);
  auto transform_system = std::make_shared<stella::systems::TransformSystem>();
  auto particle_system = std::make_shared<stella::systems::ParticleSystem>();
  auto movement_system = std::make_shared<stella::systems::MovementSystem>();

  this->add_system<stella::systems::PhysicsSystem>(physics_system);
  this->add_system<stella::systems::PlayerSystem>(player_system);
  this->add_system<stella::systems::ScrollSystem>(scroll_system);
  this->add_system<stella::systems::TiledScrollSystem>(tiled_scroll_system);
  this->add_system<stella::systems::TileSystem>(tile_system);
  this->add_system<stella::systems::TransformSystem>(transform_system);
  this->add_system<stella::systems::ParticleSystem>(particle_system);
  this->add_system<stella::systems::MovementSystem>(movement_system);

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
