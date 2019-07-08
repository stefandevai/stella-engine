#include "stella/core/game.h"
#include "stella/systems.h"

namespace stella
{
namespace core
{

  Game::Game(unsigned width, unsigned height, const std::string &title)
    : Application(width, height, title)
  {
    m_script_api.set_function("update_camera", &Game::update_camera, this);
    m_script_api.set_function("load_texture", &Game::load_texture, this);
    this->create_camera(0.0, 0.0, 0.0);
  }

  Game::~Game() { }

  void Game::load()
  {

  }

  void Game::update_systems(const double dt)
  {
    for (auto &s : m_systems)
    {
      s->update(m_registry, dt);
    }
  }

  void Game::create_camera(const double x, const double y, const double z)
  {
    m_registry.assign<stella::components::CameraComponent>(m_camera);
    m_registry.assign<stella::components::PositionComponent>(m_camera, x, y, z);
  }

  void Game::update_camera(const double x, const double y, const double z)
  {
    auto &pos = m_registry.get<stella::components::PositionComponent>(m_camera);
    pos.x = x;
    pos.y = y;
    pos.z = z;
  }

  void Game::load_texture(const std::string &name, const std::string &path)
  {
    m_textures.load(name, path);
  }

  void Game::load_font(const std::string &name, const std::string &path)
  {

  }
} // namespace script
} // namespace st


