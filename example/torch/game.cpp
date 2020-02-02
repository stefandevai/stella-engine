//#include "game.h"

//#include <tuple>
//#include <utility>
//#include <sstream>
//#include <iomanip>
//#include <functional>

//#include <stella/systems.h>
//#include <stella/components.h>

// Sandbox::Sandbox(stella::graphics::Display &display) : Display(display) {
// this->SoundPlayer.AddStream("dawn-pollen",
// "assets/audio/st-dawn_pollen.ogg"); this->SoundPlayer.Play("dawn-pollen",
// true);

// this->create_camera(0.f, 0.f, 0.f);
// srand(time(nullptr));

// this->scriptApi.vm.set_function("load_texture", &Sandbox::LoadTexture, this);
// this->scriptApi.vm.set_function("load_font", &Sandbox::LoadFont, this);
// this->scriptApi.vm.set_function("create_layer", &Sandbox::create_layer,
// this); this->scriptApi.vm.set_function("e_create_entity",
// &Sandbox::create_entity, this);
// this->scriptApi.vm.set_function("e_add_component", &Sandbox::add_component,
// this); this->scriptApi.vm.set_function("update_camera",
// &Sandbox::update_camera, this);
// this->scriptApi.vm.set_function("get_position", &Sandbox::get_position,
// this); this->scriptApi.vm.set_function("get_perlin_int",
// &Sandbox::get_perlin_int, this);
// this->scriptApi.vm.set_function("get_random_int", &Sandbox::get_random,
// this); this->scriptApi.RunScript("scripts/main.lua");
// this->scriptApi.RunLoad();

//////this->load_game_info();
//}

// Sandbox::~Sandbox() {
// for (auto& tex: this->Textures)
// delete tex.second;
//}

// void Sandbox::update_systems(const double &dt)
//{
// this->scriptApi.RunUpdate(static_cast<double>(dt));
// this->scriptApi.RunRender(static_cast<double>(dt));
// m_render_system.update(this->Registry, static_cast<double>(dt));
// m_animation_system.update(this->Registry, static_cast<double>(dt));
// m_physics_system.update(this->Registry, static_cast<double>(dt));
// m_player_system.update(this->Registry, static_cast<double>(dt));
// m_scroll_system.update(this->Registry, static_cast<double>(dt));
// m_tiled_scroll_system.update(this->Registry, static_cast<double>(dt));
// m_tile_system.update(this->Registry, static_cast<double>(dt));
// m_transform_system.update(this->Registry, static_cast<double>(dt));
// m_particle_system.update(this->Registry, static_cast<double>(dt));
// m_movement_system.update(this->Registry, static_cast<double>(dt));
// this->SoundPlayer.Update();

//////if (this->FPSText && this->Display.GetFrame() % 30 == 0) {
//////std::stringstream fps_string("");
//////fps_string << std::setprecision(4) << 1/dt << " FPS";
//////auto text = this->FPSText.component<stella::components::Text>();
//////text->Text = fps_string.str();
//////}
//////if (this->Display.GetFrame() % 30 == 0) {
//////std::cout << this->Display.getFPS() << "\n";
//////}
//}

// void Sandbox::Update(const double dt) {
// this->update_systems(dt);
//}

// void Sandbox::LoadTexture(std::string tex_name, const char *tex_path) {
// stella::graphics::Texture *texture = new stella::graphics::Texture(tex_name,
// tex_path); std::pair<std::string, stella::graphics::Texture*>
// cached_texture(tex_name, texture); this->Textures.insert(cached_texture);
//}

// void Sandbox::LoadFont(std::string font_name, const char *font_path) {
// stella::graphics::Texture *texture = new stella::graphics::Texture(font_name,
// font_path); std::pair<std::string, stella::graphics::Texture*> font(font_name,
// texture); this->Fonts.insert(font);
//}

// void Sandbox::load_game_info() {
//// Fonts
////auto title_text = entities.create();
////title_text.assign<stella::components::Position>(30.f, 30.f);
////title_text.assign<stella::components::Dimension>(9.f, 9.f);
////title_text.assign<stella::components::Text>("- TORCH -",
///"font-cursive", true);

////const unsigned char* renderer = this->Display.GetGlRenderer();
////std::stringstream renderer_string("");
////renderer_string << renderer;
////auto renderer_info = entities.create();
////renderer_info.assign<stella::components::Position>(30.f, 60.f);
////renderer_info.assign<stella::components::Dimension>(9.f, 9.f);
////renderer_info.assign<stella::components::Text>(renderer_string.str(),
///"font-cursive", true);

////const unsigned char* version = this->Display.GetGlVersion();
////std::stringstream version_string("");
////version_string << "OpenGL " << version;
////auto opengl_info = entities.create();
////opengl_info.assign<stella::components::Position>(30.f, 75.f);
////opengl_info.assign<stella::components::Dimension>(9.f, 9.f);
////opengl_info.assign<stella::components::Text>(version_string.str(),
///"font-cursive", true);

////this->FPSText = entities.create();
////this->FPSText.assign<stella::components::Position>(30.f, 90.f);
////this->FPSText.assign<stella::components::Dimension>(9.f, 9.f);
////this->FPSText.assign<stella::components::Text>("", "font-cursive");
//}

// void Sandbox::create_camera(double x, double y, double z) {
// this->Registry.assign<stella::components::Camera>(m_camera);
// this->Registry.assign<stella::components::Position>(m_camera, x, y,
// z);
//}

// void Sandbox::update_camera(double x, double y, double z) {
// auto &pos =
// this->Registry.get<stella::components::Position>(m_camera); pos.x =
// x; pos.y = y; pos.z = z;
//}

// std::tuple<int,int,int> Sandbox::get_position(entt::registry::entity_type
// entity)
//{
// if (this->Registry.has<stella::components::Position>(entity))
//{
// const auto &position =
// this->Registry.get<stella::components::Position>(entity); return
// std::make_tuple(position.x, position.y, position.z);
//}
// else
//{
// std::cout << "Entity has no Position\n";
// return std::make_tuple(0,0,0);
//}
//}

// const entt::registry::entity_type Sandbox::create_entity()
//{
// auto entity = this->Registry.create();
// return entity;
//}

// void Sandbox::create_layer(const sol::table &obj)
//{
// const std::string &layer_name = obj["name"] == sol::lua_nil ? std::string() :
// obj["name"]; const unsigned &priority = obj["priority"] == sol::lua_nil ? 0 :
// obj["priority"]; const std::string &shader_id = obj["shader"] == sol::lua_nil
// ? std::string("basic") : obj["shader"]; const bool &fixed = obj["fixed"] ==
// sol::lua_nil ? true : obj["fixed"]; auto layer = this->Registry.create();
// this->Registry.assign<stella::components::Layer>(layer, layer_name,
// priority, shader_id, fixed);
//}

// void Sandbox::add_sprite_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const std::string &layer_id = obj["layer"] == sol::lua_nil ? std::string() :
// obj["layer"]; const std::string &texture_name = obj["texture"] == sol::lua_nil
// ? std::string() : obj["texture"];

// if (!layer_id.empty() && !texture_name.empty())
//{
// if (obj["frame_dimensions"] == sol::lua_nil)
//{
// this->Registry.assign<stella::components::Sprite>(id, texture_name,
// layer_id);
//}
// else
//{
// const float &framew = obj["frame_dimensions"][1];
// const float &frameh = obj["frame_dimensions"][2];
// const unsigned &frame = obj["frame"] == sol::lua_nil ? 0 : obj["frame"];
// this->Registry.assign<stella::components::Sprite>(id, texture_name,
// glm::vec2(framew, frameh), layer_id, frame);
//}
//}
// else
//{
// std::cout << "You must provide a layer and a texture.\n";
//}
//}

// void Sandbox::add_position_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const int &x = obj[1] == sol::lua_nil ? 0 : obj[1];
// const int &y = obj[2] == sol::lua_nil ? 0 : obj[2];
// const int &z = obj[3] == sol::lua_nil ? 0 : obj[3];
// this->Registry.assign<stella::components::Position>(id, x, y, z);
//}

// void Sandbox::add_dimension_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const unsigned w = obj[1] == sol::lua_nil ? 0 : obj[1];
// const unsigned h = obj[2] == sol::lua_nil ? 0 : obj[2];
// this->Registry.assign<stella::components::Dimension>(id, w, h);
//}

// void Sandbox::add_animation_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const float &framew = obj["frame_dimensions"][1];
// const float &frameh = obj["frame_dimensions"][2];

// std::vector<std::tuple<std::string, std::vector<unsigned int>, unsigned int>>
// animations; sol::table animations_obj = obj["animations"]; for (const auto&
// key_value_pair : animations_obj)
//{
// const sol::table &animation = key_value_pair.second;
// const std::string &name = animation[1];
// const sol::table &frames_table = animation[2];
// const int &frames_table_size = frames_table.size();
// std::vector<unsigned int> frames(frames_table_size);
// for (int i = 1; i < frames_table_size+1; ++i)
//{
// int frame = frames_table[i];
// frames[i-1] = frame;
//}
// const unsigned int &speed = animation[3];
// assert(speed > 0);
// assert(frames.size() > 0);
// animations.emplace_back(name, frames, speed);
//}

// this->Registry.assign<stella::components::Animation>(id,
// animations, glm::vec2(framew, frameh));
//}

// void Sandbox::add_tileview_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// this->Registry.assign<stella::components::Tileview>(id);
//}

// void Sandbox::add_movement_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// glm::vec2 speed = obj["speed"] == sol::lua_nil ? glm::vec2() :
// glm::vec2(obj["speed"][1], obj["speed"][2]); const bool &gravity =
// obj["has_gravity"] == sol::lua_nil ? true : obj["has_gravity"]; const bool
// &constant_velocity = obj["has_constant_velocity"] == sol::lua_nil ? false :
// obj["has_constant_velocity"];
// this->Registry.assign<stella::components::Movement>(id, speed,
// gravity, constant_velocity);
//}

// void Sandbox::add_player_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// this->Registry.assign<stella::components::Player>(id);
//}

// void Sandbox::add_body_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const bool &collide_with_borders = obj["collide_with_borders"] ==
// sol::lua_nil ? false : obj["collide_with_borders"]; if (obj["drag"] ==
// sol::lua_nil)
//{
// this->Registry.assign<stella::components::Body2D>(id);
//}
// else
//{
// double dragx = obj["drag"][1];
// double dragy = obj["drag"][2];
// this->Registry.assign<stella::components::Body2D>(id,
// std::vector<double>(dragx, dragy), collide_with_borders);
//}
//}

// void Sandbox::add_text_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// const std::string &text = obj["text"];
// const std::string &font_name = obj["font_name"];
// const bool &is_static = obj["is_static"];
// this->Registry.assign<stella::components::Text>(id, text, font_name,
// is_static);
//}

// void Sandbox::add_particle_emitter_component(entt::registry::entity_type id,
// const sol::table &obj)
//{
// const std::string &type = obj["type"];
// const unsigned int &quantity = obj["quantity"];
// std::cout << type << '\n';
// std::cout << quantity << '\n';
// stella::components::ParticleEmitter::Type emitter_type;
// if (type == "fire")
//{
// emitter_type = stella::components::ParticleEmitter::Type::FIRE_EMITTER;
//}
// else if (type == "snow")
//{
// emitter_type = stella::components::ParticleEmitter::Type::SNOW_EMITTER;
//}
// this->Registry.assign<stella::components::ParticleEmitter>(id, emitter_type,
// quantity);
//}

// void Sandbox::add_tile_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// this->Registry.assign<stella::components::Tile>(id);
//}

// void Sandbox::add_scroll_component(entt::registry::entity_type id, const
// sol::table &obj)
//{
// glm::vec2 speed = obj == sol::lua_nil ? glm::vec2(0.f, 0.f) :
// glm::vec2(obj[1], obj[2]);
// this->Registry.assign<stella::components::Scroll>(id, speed);
//}

// void Sandbox::add_component(const sol::table& obj)
//{
// if (obj["type"] != sol::lua_nil)
//{
// const std::string &ct = obj["type"];
// entt::registry::entity_type id = obj["id"];

// if (ct == "sprite") add_sprite_component(id, obj["args"]);
// else if (ct == "position") add_position_component(id, obj["args"]);
// else if (ct == "dimension") add_dimension_component(id, obj["args"]);
// else if (ct == "animation") add_animation_component(id, obj["args"]);
// else if (ct == "tile") add_tile_component(id, obj["args"]);
// else if (ct == "body") add_body_component(id, obj["args"]);
// else if (ct == "text") add_text_component(id, obj["args"]);
// else if (ct == "movement") add_movement_component(id, obj["args"]);
// else if (ct == "tileview") add_tileview_component(id, obj["args"]);
// else if (ct == "particle_emitter") add_particle_emitter_component(id,
// obj["args"]); else if (ct == "scroll") add_scroll_component(id, obj["args"]);
// else if (ct == "player") add_player_component(id, obj["args"]);
// else std::cout << "ERROR: No component named " << ct << '\n';
//}
// else
//{
// std::cout << "ERROR: Please add a non nil component type\n";
//}
//}
