#include "game.h" 

#include <tuple>
#include <utility>
#include <sstream>
#include <iomanip>
#include <functional>

#include <stella/systems.h>
#include <stella/components.h>

Game::Game(stella::graphics::Display &display, int argc, char *argv[]) : Display(display) {
  this->SoundPlayer.AddStream("dawn-pollen", "assets/audio/st-dawn_pollen.ogg");
  this->SoundPlayer.Play("dawn-pollen", true);

  this->create_camera(0.f, 0.f, 0.f);
  srand(time(nullptr));

  this->scriptApi.vm.set_function("load_texture", &Game::LoadTexture, this);
  this->scriptApi.vm.set_function("load_font", &Game::LoadFont, this);
  this->scriptApi.vm.set_function("create_layer", &Game::create_layer, this);
  this->scriptApi.vm.set_function("e_create_entity", &Game::create_entity, this);
  this->scriptApi.vm.set_function("e_add_component", &Game::add_component, this);
  //this->scriptApi.vm.set_function("create_camera", &Game::create_camera, this);
  this->scriptApi.vm.set_function("update_camera", &Game::update_camera, this);
  this->scriptApi.vm.set_function("get_position", &Game::get_position, this);
  this->scriptApi.vm.set_function("get_perlin_int", &Game::get_perlin_int, this);
  ////this->scriptApi.vm.set_function("get_perlin_decimal", &Game::get_perlin_double, this);
  this->scriptApi.vm.set_function("get_random_int", &Game::get_random, this);
  this->scriptApi.RunScript("scripts/main.lua");
  this->scriptApi.RunLoad();

  //// Load game entities
  ////this->load_game_info();

  ////layer2 = entities.create();
  ////layer2.assign<stella::components::LayerComponent>("ui", 1, "text");
  ////layer1 = entities.create();
  ////layer1.assign<stella::components::LayerComponent>("basic", 0, "basic");
  ////layer3 = entities.create();
  ////layer3.assign<stella::components::LayerComponent>("particles", 1, "bloom");
 

  //// Add systems
  ////systems.add<stella::systems::CollisionSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight());
  ////systems.add<stella::systems::ParticleSystem>();
  //systems.add<stella::systems::PhysicsSystem>(this->Camera);
  //systems.add<stella::systems::ScrollSystem>();
  //systems.add<stella::systems::RenderingSystem>(this->Textures, this->Display);
  //systems.add<stella::systems::TileviewSystem>((int)this->Display.GetWidth());
  //systems.add<stella::systems::PlayerMovementSystem>((int)this->Display.GetWidth(), display, this->SoundPlayer);
  ////systems.add<stella::systems::TransformSystem>();
  ////systems.add<stella::systems::TorchSystem>(player, entities);
  //systems.add<stella::systems::AnimationSystem>();
  //systems.add<stella::systems::TilesSystem>(this->Camera);
  ////systems.add<stella::systems::GuiRenderingSystem>((int)this->Display.GetWidth(), (int)this->Display.GetHeight(), this->Fonts);
  //systems.configure();

  ////std::function<void(double)> update_function = [=](double dt) {
    ////this->update_systems(dt);
  ////};
  ////this->scriptApi.SetFunction<double, void>("update_game", update_function);
}

Game::~Game() {
	for (auto& tex: this->Textures)
		delete tex.second;
}

void Game::update_systems(const double &dt)
{
  ////const auto& camera_pos = this->Camera.component<stella::components::PositionComponent>();

  ////systems.update<stella::systems::CollisionSystem>(dt);
  ////systems.update<stella::systems::ParticleSystem>(dt);
  //systems.update<stella::systems::PhysicsSystem>(dt);
  //systems.update<stella::systems::ScrollSystem>(dt);
  //systems.update<stella::systems::RenderingSystem>(dt);
  //systems.update<stella::systems::TileviewSystem>(dt);
  //systems.update<stella::systems::PlayerMovementSystem>(dt);
  ////systems.update<stella::systems::TransformSystem>(dt);
  ////systems.update<stella::systems::TorchSystem>(dt);
  //systems.update<stella::systems::AnimationSystem>(dt);
  //systems.update<stella::systems::TilesSystem>(dt);
  ////systems.update<stella::systems::GuiRenderingSystem>(dt);

  ////if (this->FPSText && this->Display.GetFrame() % 30 == 0) {
    ////std::stringstream fps_string("");
    ////fps_string << std::setprecision(4) << 1/dt << " FPS";
    ////auto text = this->FPSText.component<stella::components::TextComponent>();
    ////text->Text = fps_string.str();
  ////}
  ////if (this->Display.GetFrame() % 30 == 0) {
    ////std::cout << this->Display.getFPS() << "\n";
  ////}
}


void Game::Update(double dt) { 
  this->scriptApi.RunUpdate(static_cast<double>(dt));
  this->scriptApi.RunRender(static_cast<double>(dt));
  m_render_system.update(this->Registry, static_cast<double>(dt));
  m_animation_system.update(this->Registry, static_cast<double>(dt));
  m_physics_system.update(this->Registry, static_cast<double>(dt));
  m_player_system.update(this->Registry, static_cast<double>(dt));
  m_scroll_system.update(this->Registry, static_cast<double>(dt));
  m_tiled_scroll_system.update(this->Registry, static_cast<double>(dt));
  m_tile_system.update(this->Registry, static_cast<double>(dt));
  m_transform_system.update(this->Registry, static_cast<double>(dt));
  m_particle_system.update(this->Registry, static_cast<double>(dt));
  this->SoundPlayer.Update();
  //this->update_systems(dt);
}

void Game::LoadTexture(std::string tex_name, const char *tex_path) {
	stella::graphics::Texture *texture = new stella::graphics::Texture(tex_name, tex_path);
	std::pair<std::string, stella::graphics::Texture*> cached_texture(tex_name, texture);
	this->Textures.insert(cached_texture);
}

void Game::LoadFont(std::string font_name, const char *font_path) {
	stella::graphics::Texture *texture = new stella::graphics::Texture(font_name, font_path);
	std::pair<std::string, stella::graphics::Texture*> font(font_name, texture);
	this->Fonts.insert(font);
}

void Game::load_game_info() {
  // Fonts
	//auto title_text = entities.create();
  //title_text.assign<stella::components::PositionComponent>(30.f, 30.f);
  //title_text.assign<stella::components::DimensionComponent>(9.f, 9.f);
	//title_text.assign<stella::components::TextComponent>("- TORCH -", "font-cursive", true);

  //const unsigned char* renderer = this->Display.GetGlRenderer();
  //std::stringstream renderer_string("");
  //renderer_string << renderer;
  //auto renderer_info = entities.create();
  //renderer_info.assign<stella::components::PositionComponent>(30.f, 60.f);
  //renderer_info.assign<stella::components::DimensionComponent>(9.f, 9.f);
  //renderer_info.assign<stella::components::TextComponent>(renderer_string.str(), "font-cursive", true);

  //const unsigned char* version = this->Display.GetGlVersion();
  //std::stringstream version_string("");
  //version_string << "OpenGL " << version;
  //auto opengl_info = entities.create();
  //opengl_info.assign<stella::components::PositionComponent>(30.f, 75.f);
  //opengl_info.assign<stella::components::DimensionComponent>(9.f, 9.f);
  //opengl_info.assign<stella::components::TextComponent>(version_string.str(), "font-cursive", true);

  //this->FPSText = entities.create();
  //this->FPSText.assign<stella::components::PositionComponent>(30.f, 90.f);
  //this->FPSText.assign<stella::components::DimensionComponent>(9.f, 9.f);
  //this->FPSText.assign<stella::components::TextComponent>("", "font-cursive");
}

void Game::create_camera(double x, double y, double z) {
  this->Registry.assign<stella::components::CameraComponent>(m_camera);
  this->Registry.assign<stella::components::PositionComponent>(m_camera, x, y, z);
}

void Game::update_camera(double x, double y, double z) {
  auto &pos = this->Registry.get<stella::components::PositionComponent>(m_camera);
  pos.x = x;
  pos.y = y;
  pos.z = z;
}

std::tuple<int,int,int> Game::get_position(entt::registry::entity_type entity)
{
  if (this->Registry.has<stella::components::PositionComponent>(entity))
  {
    const auto &position = this->Registry.get<stella::components::PositionComponent>(entity);
    return std::make_tuple(position.x, position.y, position.z);
  }
  else
  {
    std::cout << "Entity has no PositionComponent\n";
    return std::make_tuple(0,0,0);
  }
}

const entt::registry::entity_type Game::create_entity()
{
  auto entity = this->Registry.create();
  return entity;

  //auto entity = entities.create();
  //auto eid = entity.id();
  //return std::tuple<unsigned int, unsigned int>(eid.index(), eid.version());
}

void Game::create_layer(const sol::table &obj)
{
  const std::string &layer_name = obj["name"] == sol::lua_nil ? std::string() : obj["name"];
  const unsigned &priority = obj["priority"] == sol::lua_nil ? 0 : obj["priority"];
  const std::string &shader_id = obj["shader"] == sol::lua_nil ? std::string("basic") : obj["shader"];
  const bool &fixed = obj["fixed"] == sol::lua_nil ? true : obj["fixed"];
  auto layer = this->Registry.create();
  this->Registry.assign<stella::components::LayerComponent>(layer, layer_name, priority, shader_id, fixed);
}

void Game::add_sprite_component(entt::registry::entity_type id, const sol::table &obj)
{
  const std::string &layer_id = obj["layer"] == sol::lua_nil ? std::string() : obj["layer"];
  const std::string &texture_name = obj["texture"] == sol::lua_nil ? std::string() : obj["texture"];

  if (!layer_id.empty() && !texture_name.empty())
  {
    if (obj["frame_dimensions"] == sol::lua_nil)
    {
      this->Registry.assign<stella::components::SpriteComponent>(id, texture_name, layer_id);
    }
    else
    {
      const float &framew = obj["frame_dimensions"][1];
      const float &frameh = obj["frame_dimensions"][2];
      const unsigned &frame = obj["frame"] == sol::lua_nil ? 0 : obj["frame"];
      this->Registry.assign<stella::components::SpriteComponent>(id, texture_name, glm::vec2(framew, frameh), layer_id, frame);
    }
  }
  else
  {
    std::cout << "You must provide a layer and a texture.\n";
  }
}

void Game::add_position_component(entt::registry::entity_type id, const sol::table &obj)
{
  const int &x = obj[1] == sol::lua_nil ? 0 : obj[1];
  const int &y = obj[2] == sol::lua_nil ? 0 : obj[2];
  const int &z = obj[3] == sol::lua_nil ? 0 : obj[3];
  this->Registry.assign<stella::components::PositionComponent>(id, x, y, z);
}

void Game::add_dimension_component(entt::registry::entity_type id, const sol::table &obj)
{
  const unsigned w = obj[1] == sol::lua_nil ? 0 : obj[1];
  const unsigned h = obj[2] == sol::lua_nil ? 0 : obj[2];
  this->Registry.assign<stella::components::DimensionComponent>(id, w, h);
}
 
void Game::add_animation_component(entt::registry::entity_type id, const sol::table &obj)
{
  const float &framew = obj["frame_dimensions"][1];
  const float &frameh = obj["frame_dimensions"][2];

  std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>> animations;
  sol::table animations_obj = obj["animations"];
  for (const auto& key_value_pair : animations_obj)
  {
    const sol::table &animation = key_value_pair.second;
    const std::string &name = animation[1];
    const sol::table &frames_table = animation[2];
    const int &frames_table_size = frames_table.size();
    std::vector<unsigned int> frames(frames_table_size);
    for (int i = 1; i < frames_table_size+1; ++i)
    {
      int frame = frames_table[i];
      frames[i-1] = frame;
    }
    const unsigned int &speed = animation[3];
    assert(speed > 0);
    assert(frames.size() > 0);
    animations.emplace_back(name, frames, speed);
  }

  this->Registry.assign<stella::components::AnimationsComponent>(id, animations, glm::vec2(framew, frameh));
}

void Game::add_tileview_component(entt::registry::entity_type id, const sol::table &obj)
{
  this->Registry.assign<stella::components::TileviewComponent>(id);
}

void Game::add_movement_component(entt::registry::entity_type id, const sol::table &obj)
{
  glm::vec2 speed = obj["speed"] == sol::lua_nil ? glm::vec2() : glm::vec2(obj["speed"][1], obj["speed"][2]);
  const bool &gravity = obj["has_gravity"] == sol::lua_nil ? true : obj["has_gravity"];
  const bool &constant_velocity = obj["has_constant_velocity"] == sol::lua_nil ? false : obj["has_constant_velocity"];
  this->Registry.assign<stella::components::MovementComponent>(id, speed, gravity, constant_velocity);
}

void Game::add_player_component(entt::registry::entity_type id, const sol::table &obj)
{
  this->Registry.assign<stella::components::PlayerComponent>(id);
}

void Game::add_body_component(entt::registry::entity_type id, const sol::table &obj)
{
  const bool &collide_with_borders = obj["collide_with_borders"] == sol::lua_nil ? false : obj["collide_with_borders"];
  if (obj["drag"] == sol::lua_nil)
  {
    this->Registry.assign<stella::components::Body2DComponent>(id);
  }
  else
  {
    double dragx = obj["drag"][1];
    double dragy = obj["drag"][2];
    this->Registry.assign<stella::components::Body2DComponent>(id, std::vector<double>(dragx, dragy), collide_with_borders);
  }
}

void Game::add_text_component(entt::registry::entity_type id, const sol::table &obj)
{
  const std::string &text = obj["text"];
  const std::string &font_name = obj["font_name"];
  const bool &is_static = obj["is_static"];
  this->Registry.assign<stella::components::TextComponent>(id, text, font_name, is_static);
}

void Game::add_particle_emitter_component(entt::registry::entity_type id, const sol::table &obj)
{
  const std::string &type = obj["type"];
  const unsigned int &quantity = obj["quantity"];
  std::cout << type << '\n';
  std::cout << quantity << '\n';
  stella::components::ParticleEmitter::Type emitter_type;
  if (type == "fire")
  {
    emitter_type = stella::components::ParticleEmitter::Type::FIRE_EMITTER;
  }
  else if (type == "snow")
  {
    emitter_type = stella::components::ParticleEmitter::Type::SNOW_EMITTER;
  }
  this->Registry.assign<stella::components::ParticleEmitter>(id, emitter_type, quantity);
}

void Game::add_tile_component(entt::registry::entity_type id, const sol::table &obj)
{
  this->Registry.assign<stella::components::TileComponent>(id);
}

void Game::add_scroll_component(entt::registry::entity_type id, const sol::table &obj)
{
  glm::vec2 speed = obj == sol::lua_nil ? glm::vec2(0.f, 0.f) : glm::vec2(obj[1], obj[2]);
  this->Registry.assign<stella::components::ScrollComponent>(id, speed);
}

void Game::add_component(const sol::table& obj)
{
  if (obj["type"] != sol::lua_nil)
  {
    const std::string &ct = obj["type"];
    entt::registry::entity_type id = obj["id"];

    if (ct == "sprite") add_sprite_component(id, obj["args"]);
    else if (ct == "position") add_position_component(id, obj["args"]);
    else if (ct == "dimension") add_dimension_component(id, obj["args"]);
    else if (ct == "animation") add_animation_component(id, obj["args"]);
    else if (ct == "tile") add_tile_component(id, obj["args"]);
    else if (ct == "body") add_body_component(id, obj["args"]);
    else if (ct == "text") add_text_component(id, obj["args"]);
    else if (ct == "movement") add_movement_component(id, obj["args"]);
    else if (ct == "tileview") add_tileview_component(id, obj["args"]);
    else if (ct == "particle_emitter") add_particle_emitter_component(id, obj["args"]);
    else if (ct == "scroll") add_scroll_component(id, obj["args"]);
    else if (ct == "player") add_player_component(id, obj["args"]);
    else std::cout << "ERROR: No component named " << ct << '\n';
  }
  else
  {
    std::cout << "ERROR: Please add a non nil component type\n";
  }
}

