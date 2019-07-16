#include "sandbox.h"

//#include <sstream>
//#include <iomanip>

Sandbox::Sandbox()
  : stella::core::Game(896, 504, "Stella Engine")
{
  //this->add_system<stella::systems::PhysicsSystem>(m_tile_map, m_registry);
  //this->add_system<stella::systems::ScrollSystem>();
  //this->add_system<stella::systems::TiledScrollSystem>(m_initial_width);
  //this->add_system<stella::systems::TileSystem>(m_tile_map, m_camera, m_registry);
  //this->add_system<stella::systems::ParticleSystem>();
  //this->add_system<stella::systems::MovementSystem>();
  //this->add_system<stella::systems::TransformSystem>();
  //this->add_system<stella::systems::TextSystem>(m_registry);

  test_world.add_water_surface(water_surface);
  for (unsigned i = 0; i < water_surface->number_of_columns(); ++i)
  {
    auto shape = std::make_shared<stella::graphics::Shape>(std::vector<glm::vec2>{glm::vec2(0.f,0.f), glm::vec2(water_surface->column_width(), 0.f), glm::vec2(water_surface->column_width(), water_surface->height()), glm::vec2(0.f, water_surface->height())},
                                                                //glm::vec3(10 + water_surface->column_width()*i, 200.f, 1.f),
                                                                glm::vec3(water_surface->column_width()*i, 504.f - 128.f, 1.f),
                                                                glm::vec4(0.2f, 0.4f, 0.9f, 0.6f));
    water_layer.Add(shape);
    water_shapes.emplace_back(shape);
  }
  water_surface->perturbate(water_surface->width()/2 -20, -20.0);
  
  //m_script_api.set_function("e_get_player_id", [this]() {
      //return m_player.entity;
  //});
  //m_script_api.run_script("./scripts/main.lua");
  //m_script_api.run_function("load_game");

  //m_tile_map.create_tile_entities(0, m_display.GetWidth(), 0, m_display.GetHeight());

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
  test_world.Update(dt);

  const auto number_of_columns = water_surface->number_of_columns();
  for (unsigned i = 0; i < number_of_columns - 1; ++i)
  {
    const auto first_height = water_surface->column_height(i);
    const auto second_height = water_surface->column_height(i+1);
    water_shapes[i]->set_vertex(0, 0.f, -first_height);
    water_shapes[i]->set_vertex(1, water_surface->column_width(), -second_height);
  }

  const auto last_height = water_surface->column_height(number_of_columns-1);
  water_shapes[number_of_columns-1]->set_vertex(0, 0.f, -last_height);
  water_layer.Render();

  //m_player.update();
  //m_script_api.run_function("update_game", dt);
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
