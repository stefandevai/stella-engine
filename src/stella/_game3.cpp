//#include "stella/game.hpp"
//#include "stella/systems.hpp"
////#include "stella/graphics/sprite_renderer.hpp"

// namespace stella
//{
// namespace core
//{
// Game::Game (unsigned width, unsigned height, const std::string& title) : Application (width, height, title)
//{
// m_script_api.set_function ("update_camera", &Game::update_camera, this);
// m_script_api.set_function ("load_texture", &Game::load_texture, this);
// m_script_api.set_function ("load_font", &Game::load_font, this);
// this->create_camera (0.0, 0.0, 0.0);
//}

// Game::~Game() {}

// void Game::load() {}
// void Game::update() {}
// void Game::render() {}

// void Game::update_systems (const double dt)
//{
// for (auto& s : m_systems)
//{
// s->update (m_registry, dt);
//}
//}

// void Game::create_camera (const double x, const double y, const double z)
//{
// m_registry.emplace<stella::component::Camera> (m_camera);
// m_registry.emplace<stella::component::Position> (m_camera, x, y, z);
// m_registry.emplace<stella::component::Dimension> (m_camera, m_initial_width + x, m_initial_height + y);
//}

// void Game::update_camera (const double x, const double y, const double z)
//{
// auto& pos = m_registry.get<stella::component::Position> (m_camera);
// pos.x     = x;
// pos.y     = y;
// pos.z     = z;
//}

// std::vector<float> Game::get_camera_pos()
//{
// auto& pos = m_registry.get<stella::component::Position> (m_camera);
// return std::vector<float>{pos.x, pos.y, pos.z};
//}

// void
// Game::load_texture (const std::string& name, const std::string& path, const unsigned hframes, const unsigned vframes)
//{
// m_textures.load (name, path, hframes, vframes);
//}

// void Game::load_font (const std::string& name, const std::string& path, const unsigned size)
//{
// m_fonts.load (name, path, size);
//}
//} // namespace core
//} // namespace stella
