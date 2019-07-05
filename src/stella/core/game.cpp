#include "stella/core/game.h"

namespace stella
{
namespace core
{

  Game::Game(unsigned width, unsigned height, const std::string &title)
    : Application(width, height, title)
  {
  }

  Game::~Game() { }

  void Game::load()
  {

  }

  void Game::update(const double dt)
  {
    this->update_systems(dt);
  }

  void Game::add_system(systems::System& game_system)
  {
    m_systems.push_back(game_system);
  }

  void Game::update_systems(const double dt)
  {
    for (auto &s : m_systems)
    {
      s.update(m_registry, dt);
    }
  }

  void Game::load_texture(std::string texture_id, const char *texture_path)
  {
  }

  void Game::load_font(std::string font_id, const char *font_path)
  {
  }


} // namespace script
} // namespace st


