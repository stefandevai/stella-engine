#pragma once

#include "../stella/graphics/shape.h"
#include "../stella/graphics/framebuffer.h"
#include "widgets/chat.h"
#include "./debug_layer.h"
#include "widgets/console.h"
#include "widgets/inspector.h"
#include "./log_system.h"
#include "widgets/map_editor.h"
#include "widgets/tileset_editor.h"
#include "widgets/scene.h"
#include <entt/entity/registry.hpp>
//#include "../stella/core/game.h"
#include "state.h"
#include "widgets/toolbar.h"
#include "../../lib/imgui/imgui.h"
#include "../../lib/imgui/examples/imgui_impl_opengl3.h"
#include "../../lib/imgui/examples/imgui_impl_sdl.h"

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
  class EditorGui
  {
  private:
    static const ImWchar ICON_FA_MIN = 0xf044;
    static const ImWchar ICON_FA_MAX = 0xf245;

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
    widget::Chat m_chat{m_window_flags, m_font_mono};
    LogSystem m_log_system{m_console};
    entt::registry& m_registry;
    DebugLayer m_debug_layer{896, 504, true};
    entt::entity m_editor_layer  = entt::null;
    entt::entity m_editor_sprite = entt::null;
    widget::TilesetEditor m_tileset_editor{"assets/sprites/tilesetv2.png"};
    widget::MapEditor m_map_editor{m_game};
    widget::Inspector m_inspector;
    widget::Toolbar m_toolbar;
    float m_window_width = 0.f, m_window_height = 0.f, m_game_width = 0.f, m_game_height = 0.f;

    std::vector<glm::vec2> vertices{
        glm::vec2{0.0f, 20.0f}, glm::vec2{128.0f, 80.0f}, glm::vec2{128.0f, 128.0f}, glm::vec2{0.0f, 128.0f}};
    std::shared_ptr<graphics::Shape> shape = std::make_shared<graphics::Shape> (vertices, glm::vec3{200.f, 100.f, 1.f});

    // View options
    bool m_view_physics_debug_layer = false;
    bool m_show_editor = true;

    std::unique_ptr<graphics::Framebuffer> m_FBO;
    widget::Scene m_scene;

  public:
    // EditorGui(entt::registry& registry);
    EditorGui (nikte::Game& game);
    ~EditorGui();
    // void init (SDL_Window* window, SDL_GLContext gl_context, const char* glsl_version);
    void init();
    void configure_input();
    void render (const float window_width, const float window_height, const float game_width, const float game_height);
    void update();
    void run();

  private:
    void init_style();
    void
    draw_dock (const float window_width, const float window_height, const float game_width, const float game_height);
    void draw_editor();
    void draw_info (const ImVec2& pos);
    void draw_menu_bar();
    void handle_state (ImGuiIO& io);
    void handle_tile_pen (ImGuiIO& io);
    void handle_inspector (ImGuiIO& io);
  };

} // namespace editor
} // namespace stella
