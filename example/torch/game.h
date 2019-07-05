#pragma once

#include <unordered_map>
#include <string>

//#include <entityx/entityx.h>
#include <stella/stella.h>
#include <stella/components.h>
#include <stella/perlin.h>

#define SOL_ALL_SAFETIES_ON 1
#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>

#include <entt/entity/registry.hpp>
#include <stella/systemss/render_system.h>
#include <stella/systemss/animation_system.h>
#include <stella/systemss/physics_system.h>
#include <stella/systemss/player_system.h>
#include <stella/systemss/scroll_system.h>
#include <stella/systemss/tiled_scroll_system.h>
#include <stella/systemss/tile_system.h>
#include <stella/systemss/transform_system.h>
#include <stella/systemss/particle_system.h>

//namespace ex = entityx;

class Game {
public:
  Game(stella::graphics::Display &display, int argc, char *argv[]);
  ~Game();

  void Update(double dt);

private:
  entt::registry Registry = entt::registry();
	stella::graphics::Display &Display;
	//ex::Entity FPSText;
  stella::ScriptApi scriptApi;
  stella::audio::SoundPlayer SoundPlayer;
  //entt::registry::entity_type m_camera = Registry.create();
  entt::registry::entity_type m_camera = Registry.create();

  //ex::Entity Camera;

	std::unordered_map<std::string, stella::graphics::Texture*> Textures;
	std::unordered_map<std::string, stella::graphics::Texture*> Fonts;

  stella::systems::RenderSystem m_render_system{Registry, Textures, Display};
  stella::systems::AnimationSystemm m_animation_system;
  stella::systems::PhysicsSystemm m_physics_system{Registry, m_camera};
  stella::systems::PlayerSystem m_player_system{(int)Display.GetWidth(), Display, SoundPlayer};
  stella::systems::ScrollSystemm m_scroll_system;
  stella::systems::TiledScrollSystem m_tiled_scroll_system{(int)Display.GetWidth()};
  stella::systems::TileSystem m_tile_system{m_camera};
  stella::systems::TransformSystemm m_transform_system;
  stella::systems::ParticleSystemm m_particle_system;

  stella::PerlinNoise NoiseGenerator = stella::PerlinNoise(47);

  void create_camera(double x, double y, double z);
  void update_camera(double x, double y, double z);
  std::tuple<int,int,int> get_position(entt::registry::entity_type entity);

	void LoadTexture(std::string tex_name, const char *tex_path);
	void LoadFont(std::string font_name, const char *font_path);

	void load_player(int x, int y);
	void load_blocks();
	void load_particles();
	void load_background();
	void load_foreground();
	void load_game_info();

  void update_systems(const double &dt);

  // Lua functions
  void create_layer(const sol::table &obj);
  const entt::registry::entity_type create_entity();
  void add_sprite_component(entt::registry::entity_type id, const sol::table &obj);
  void add_position_component(entt::registry::entity_type index, const sol::table &obj);
  void add_dimension_component(entt::registry::entity_type id, const sol::table &obj);
  void add_animation_component(entt::registry::entity_type id, const sol::table &obj);
  void add_tileview_component(entt::registry::entity_type id, const sol::table &obj);
  void add_movement_component(entt::registry::entity_type id, const sol::table &obj);
  void add_player_component(entt::registry::entity_type id, const sol::table &obj);
  void add_body_component(entt::registry::entity_type id, const sol::table &obj);
  void add_text_component(entt::registry::entity_type id, const sol::table &obj);
  void add_particle_emitter_component(entt::registry::entity_type id, const sol::table &obj);
  void add_tile_component(entt::registry::entity_type id, const sol::table &obj);
  void add_scroll_component(entt::registry::entity_type id, const sol::table &obj);
  void add_component(const sol::table &obj);

  double get_perlin_double(double x, double y)
  {
    return NoiseGenerator.get_double(x, y);
  }
  int get_perlin_int(int min, int max, int pos)
  {
    double x = static_cast<double>(pos % 256);
    double y = static_cast<double>((pos / 256) % 256);
    return NoiseGenerator.get_int(min, max, x/100.f, y/100.f);
  }
  int get_random(int min, int max)
  {
    return min + rand() % (( max + 1 ) - min);
  }
};

