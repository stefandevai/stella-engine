#pragma once

#include "../stella/graphics/shape.hpp"
#include "../stella/graphics/framebuffer.hpp"
#include "widgets/chat.hpp"
#include "stella/graphics/layers/shape_layer.hpp"
#include "widgets/console.hpp"
#include "widgets/inspector.hpp"
#include "./log_system.hpp"
#include "widgets/map_editor.hpp"
#include "widgets/tileset_editor.hpp"
#include "widgets/scene.hpp"
#include <entt/entity/registry.hpp> // IWYU pragma: export
//#include "../stella/core/game.hpp"
#include "state.hpp"
#include "widgets/toolbar.hpp"
#include "widgets/block_editor.hpp"
#include "../../lib/imgui/imgui.h"                       // IWYU pragma: export
#include "../../lib/imgui/examples/imgui_impl_opengl3.h" // IWYU pragma: export
#include "../../lib/imgui/examples/imgui_impl_sdl.h"     // IWYU pragma: export
#include "systems/selection.hpp"

struct SDL_Window;
union SDL_Event;
typedef void* SDL_GLContext;
namespace nikte
{
class Game;
}

namespace stella
{
namespace editor
{
  class Editor
  {
  private:
    // static const ImWchar ICON_FA_MIN = 0xf044;
    // static const ImWchar ICON_FA_MAX = 0xf245;

    State m_current_state = EDIT;
    Tool m_current_tool   = INSPECTOR;

    nikte::Game& m_game;
    SDL_Window* m_window = nullptr;
    ImFont *m_font_mono = nullptr, *m_font_sans_regular = nullptr, *m_font_sans_bold = nullptr;
    const ImGuiWindowFlags m_window_flags =
        // ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
        // ImGuiWindowFlags_NoTitleBar;
        ImGuiWindowFlags_NoTitleBar;
    // const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoMove |
    // ImGuiWindowFlags_NoResize;
    widget::Console m_console{m_window_flags, m_font_mono};
    widget::Chat m_chat{m_font_mono};
    entt::registry& m_registry;
    std::shared_ptr<graphics::ShapeLayerT> m_debug_layer;
    entt::entity m_editor_layer  = entt::null;
    entt::entity m_editor_sprite = entt::null;
    widget::TilesetEditor m_tileset_editor{"assets/sprites/tilesetv2.png"};
    widget::MapEditor m_map_editor{m_game};
    widget::Inspector m_inspector;
    widget::Toolbar m_toolbar;
    widget::BlockEditor m_block_editor;
    float m_window_width = 0.f, m_window_height = 0.f, m_game_width = 0.f, m_game_height = 0.f;

    std::vector<glm::vec2> vertices{
        glm::vec2{0.0f, 20.0f}, glm::vec2{128.0f, 80.0f}, glm::vec2{128.0f, 128.0f}, glm::vec2{0.0f, 128.0f}};
    std::shared_ptr<graphics::Shape> shape = std::make_shared<graphics::Shape> (vertices, glm::vec3{200.f, 100.f, 1.f});

    // View options
    bool m_view_physics_debug_layer = true;
    bool m_show_editor              = true;

    std::unique_ptr<graphics::Framebuffer> m_FBO;
    widget::Scene m_scene;

    ImVec2 camera_pos_without_pan = ImVec2();
    bool is_panning               = false;

    // Systems
    LogSystem m_log_system{m_console};
    std::shared_ptr<system::Selection> m_selection_system;
    std::vector<std::shared_ptr<system::System>> m_systems;

  public:
    // Editor(entt::registry& registry);
    Editor (nikte::Game& game);
    ~Editor();
    // void init (SDL_Window* window, SDL_GLContext gl_context, const char* glsl_version);
    void init();
    void configure_input();
    void render (const float window_width, const float window_height, const float game_width, const float game_height);
    void update (const double dt);
    void run();

  private:
    void init_style();
    void
    draw_dock (const float window_width, const float window_height, const float game_width, const float game_height);
    void draw_editor();
    void draw_info (const ImVec2& pos);
    void draw_menu_bar();
    void m_play_mode();
    void m_edit_mode();
    void m_handle_state (ImGuiIO& io);
    void m_handle_tile_pen (const ImGuiIO& io);
    void m_handle_pan_tool (const ImGuiIO& io);
    void m_handle_inspector (const ImGuiIO& io);
    void m_widget_build_option (widget::Widget& widget, const std::string& shortcut);
    void m_map_tile_pos (const ImGuiIO& io, std::function<void (const ImVec2&)> position_action);
  };

} // namespace editor
} // namespace stella
