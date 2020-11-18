#pragma once

#include "stella/systems/system.hpp"
#include "state.hpp"
#include <memory>
#include <string>
#include <entt/entity/registry.hpp> // IWYU pragma: export

// Widgets
#include "widgets/toolbar.hpp"
#include "widgets/console.hpp"
#include "widgets/chat.hpp"
#include "widgets/inspector.hpp"
#include "widgets/scene.hpp"
#include "widgets/scene_editor.hpp"
#include "widgets/new_scene_popup.hpp"

struct SDL_Window;
struct ImFont;
namespace stella
{
  class Game;
}
namespace stella
{
namespace graphics
{
  class Framebuffer;
}
}

typedef std::vector<std::shared_ptr<stella::system::System>> SystemContainer;

namespace stella
{
namespace editor
{
  class Editor
  {
  public:
    Editor (stella::Game& game);
    ~Editor();

    void render (const float window_width, const float window_height, const float game_width, const float game_height);
    void update (const double dt);
    void run();

  private:
    void m_init();
    void m_init_imgui();
    void m_deinit_imgui();
    void m_handle_input();
    void m_handle_state (ImGuiIO& io);

    // Render methods
    void m_render_menu_bar();
    void m_render_dock();
    void m_render_view_menu_option (widget::Widget& widget, const std::string& shortcut);

  private:
    // Editor related properties
    stella::Game& m_game;
    SDL_Window* m_window = nullptr;
    float m_window_width = 0.f;
    float m_window_height = 0.f;
    float m_game_width = 0.f;
    float m_game_height = 0.f;
    std::unique_ptr<graphics::Framebuffer> m_FBO;
    State m_current_state = EDIT;
    Tool m_current_tool   = INSPECTOR;

    // ImGui related properties
    ImFont* m_font_mono = nullptr;
    ImFont* m_font_sans_regular = nullptr;
    ImFont* m_font_sans_bold = nullptr;
    const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoTitleBar;

    // Entt related properties
    entt::registry m_registry;
    SystemContainer m_systems;

    // Widgets
    widget::Toolbar m_toolbar;
    widget::Scene m_scene;
    widget::SceneEditor m_scene_editor;
    widget::Console m_console{m_window_flags, m_font_mono};
    widget::Inspector m_inspector;
    widget::NewScenePopup m_new_scene_popup{m_game};

    // Hardcoded strings
    const std::string m_imgui_ini_path = "config/imgui.ini";
    const std::string m_font_mono_path = "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf";
    const std::string m_font_sans_regular_path = "assets/fonts/Lato/Lato-Regular.ttf";
    const std::string m_font_sans_bold_path = "assets/fonts/Lato/Lato-Bold.ttf";
    const std::string m_font_awesome_path = "assets/fonts/fa-solid-900.ttf";
  };
}
}
