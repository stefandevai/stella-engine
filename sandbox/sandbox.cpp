#include "sandbox.h"

//#include <sstream>
//#include <iomanip>

Sandbox::Sandbox()
  : stella::core::Game(896, 504, "Stella Engine")
{
  this->add_system<stella::systems::PhysicsSystem>(m_registry, m_camera);
  this->add_system<stella::systems::ScrollSystem>();
  this->add_system<stella::systems::TiledScrollSystem>(m_initial_width);
  this->add_system<stella::systems::TileSystem>(m_camera);
  this->add_system<stella::systems::ParticleSystem>();
  this->add_system<stella::systems::MovementSystem>();
  this->add_system<stella::systems::TransformSystem>();
  this->add_system<stella::systems::TextSystem>(m_registry);

  m_script_api.set_function("e_get_player_id", [this]() {
      return m_player.entity;
  });
  m_script_api.run_script("./scripts/main.lua");
  m_script_api.run_function("load_game");
  
  //m_sound_player.AddStream("dawn-pollen", "assets/audio/st-dawn_pollen.ogg");
  //m_sound_player.Play("dawn-pollen", true);

  //const unsigned char* renderer = m_display.GetGlRenderer();
  //std::stringstream renderer_string("");
  //renderer_string << renderer;
  //auto renderer_info = m_registry.create();
  //m_registry.assign<stella::components::PositionComponent>(renderer_info, 30.f, 60.f);
  //m_registry.assign<stella::components::DimensionComponent>(renderer_info, 9.f, 9.f);
  //m_registry.assign<stella::components::TextComponent>(renderer_info, renderer_string.str(), "font-cursive", true);

  //const unsigned char* version = m_display.GetGlVersion();
  //std::stringstream version_string("");
  //version_string << "OpenGL " << version;
  //auto opengl_info = m_registry.create();
  //m_registry.assign<stella::components::PositionComponent>(opengl_info, 30.f, 75.f);
  //m_registry.assign<stella::components::DimensionComponent>(opengl_info, 9.f, 9.f);
  //m_registry.assign<stella::components::TextComponent>(opengl_info, version_string.str(), "font-cursive", true);

  //m_registry.assign<stella::components::PositionComponent>(m_fps_text, 30.f, 90.f);
  //m_registry.assign<stella::components::DimensionComponent>(m_fps_text, 9.f, 9.f);
  //m_registry.assign<stella::components::TextComponent>(m_fps_text, "", "font-cursive");

  //m_registry.assign<stella::components::PositionComponent>(m_ms_text, 30.f, 105.f);
  //m_registry.assign<stella::components::DimensionComponent>(m_ms_text, 9.f, 9.f);
  //m_registry.assign<stella::components::TextComponent>(m_ms_text, "", "font-cursive");

  // Update once to configure systems
  this->update_systems(0.0);
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
  //m_sound_player.Update();

  //if (m_registry.valid(m_fps_text) && m_display.GetFrame() % 10 == 0)
  //{
    //std::stringstream fps_string("");
    //fps_string << std::fixed << std::setprecision(6) << m_display.getFPS() << " FPS";
    //auto &text = m_registry.get<stella::components::TextComponent>(m_fps_text);
    //text.Text = fps_string.str();
  //}

  //if (m_registry.valid(m_ms_text) && m_display.GetFrame() % 10 == 0)
  //{
    //std::stringstream ms_string("");
    //ms_string << std::fixed << std::setprecision(8) << dt << " ms";
    //auto &text = m_registry.get<stella::components::TextComponent>(m_ms_text);
    //text.Text = ms_string.str();
  //}
}
