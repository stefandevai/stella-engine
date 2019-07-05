//#pragma once

//#include <unordered_map>
//#include <string>

//#include <stella/stella.h>
//#include <stella/components.h>
//#include <stella/perlin.h>

//#include <entt/entity/registry.hpp>
//#include <stella/systems/render_system.h>
//#include <stella/systems/animation_system.h>
//#include <stella/systems/physics_system.h>
//#include <stella/systems/player_system.h>
//#include <stella/systems/scroll_system.h>
//#include <stella/systems/tiled_scroll_system.h>
//#include <stella/systems/tile_system.h>
//#include <stella/systems/transform_system.h>
//#include <stella/systems/particle_system.h>
//#include <stella/systems/movement_system.h>

//class Sandbox {
//public:
  //Sandbox(stella::graphics::Display &display);
  //~Sandbox();

  //void Update(double dt);

//private:
  //entt::registry Registry = entt::registry();
	//stella::graphics::Display &Display;
  //stella::script::Api scriptApi;
  //stella::audio::SoundPlayer SoundPlayer;
  //entt::registry::entity_type m_camera = Registry.create();

	//std::unordered_map<std::string, stella::graphics::Texture*> Textures;
	//std::unordered_map<std::string, stella::graphics::Texture*> Fonts;

  //stella::systems::RenderSystem m_render_system{Registry, Textures, Display};
  //stella::systems::AnimationSystem m_animation_system;
  //stella::systems::PhysicsSystem m_physics_system{Registry, m_camera};
  //stella::systems::PlayerSystem m_player_system{Display, SoundPlayer};
  //stella::systems::ScrollSystem m_scroll_system;
  //stella::systems::TiledScrollSystem m_tiled_scroll_system{(int)Display.GetWidth()};
  //stella::systems::TileSystem m_tile_system{m_camera};
  //stella::systems::TransformSystem m_transform_system;
  //stella::systems::ParticleSystem m_particle_system;
  //stella::systems::MovementSystem m_movement_system;

  //stella::PerlinNoise NoiseGenerator = stella::PerlinNoise(47);

  //void create_camera(double x, double y, double z);
  //void update_camera(double x, double y, double z);
  //std::tuple<int,int,int> get_position(entt::registry::entity_type entity);

	//void LoadTexture(std::string tex_name, const char *tex_path);
	//void LoadFont(std::string font_name, const char *font_path);

	//void load_player(int x, int y);
	//void load_blocks();
	//void load_particles();
	//void load_background();
	//void load_foreground();
	//void load_game_info();

  //void update_systems(const double &dt);

  //// Lua functions
  //void create_layer(const sol::table &obj);
  //const entt::registry::entity_type create_entity();
  //void add_sprite_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_position_component(entt::registry::entity_type index, const sol::table &obj);
  //void add_dimension_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_animation_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_tileview_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_movement_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_player_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_body_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_text_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_particle_emitter_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_tile_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_scroll_component(entt::registry::entity_type id, const sol::table &obj);
  //void add_component(const sol::table &obj);

  //inline double get_perlin_double(double x, double y)
  //{
    //return NoiseGenerator.get_double(x, y);
  //}

  //inline int get_perlin_int(double min, double max)
  //{
    //int pos = 0;
    //double x = static_cast<double>(pos % 256);
    //double y = static_cast<double>((pos / 256) % 256);
    //return NoiseGenerator.get_int(static_cast<int>(min), static_cast<int>(max), x/100.f, y/100.f);
  //}
  //inline int get_random(double min, double max)
  //{
    //return static_cast<int>(min) + rand() % (( static_cast<int>(max) + 1 ) - static_cast<int>(min));
  //}
//};

