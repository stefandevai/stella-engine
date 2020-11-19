#pragma once

#include "stella/systems/system.hpp"
#include "modes.hpp"
#include "actions.hpp"
#include "tools.hpp"
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
#include "widgets/load_scene_popup.hpp"
#include "widgets/edit_mode_main_menu_options.hpp"

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

    void run();

  private:
    void m_init();
    void m_init_imgui();
    void m_deinit_imgui();
    void m_run_edit_mode();
    void m_run_play_mode();
    void m_handle_edit_mode_input();
    void m_handle_play_mode_input();
    void m_handle_edit_mode_tool (ImGuiIO& io);
    void m_handle_play_mode_tool (ImGuiIO& io);
    void m_handle_edit_mode_actions ();
    void m_handle_play_mode_actions ();

    // Render methods
    void m_render_edit_mode (const float window_width, const float window_height, const float game_width, const float game_height);
    void m_render_play_mode (const float window_width, const float window_height, const float game_width, const float game_height);
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
    EditorMode m_current_mode = EditorMode::EDIT;
    EditorTool m_current_tool = EditorTool::INSPECTOR;
    Action m_current_action = Action::NONE;

    // ImGui related properties
    ImFont* m_font_mono = nullptr;
    ImFont* m_font_sans_regular = nullptr;
    ImFont* m_font_sans_bold = nullptr;
    const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoTitleBar;

    // Widgets
    widget::Toolbar m_toolbar;
    widget::Scene m_scene;
    widget::SceneEditor m_scene_editor;
    widget::Console m_console{m_window_flags, m_font_mono};
    widget::Inspector m_inspector;
    widget::NewScenePopup m_new_scene_popup{m_game};
    widget::LoadScenePopup m_load_scene_popup{m_game};
    widget::EditModeMainMenuOptions m_edit_mode_main_menu_options;

    // Hardcoded strings
    const std::string m_imgui_ini_path = "config/imgui.ini";
    const std::string m_font_mono_path = "assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf";
    const std::string m_font_sans_regular_path = "assets/fonts/Lato/Lato-Regular.ttf";
    const std::string m_font_sans_bold_path = "assets/fonts/Lato/Lato-Bold.ttf";
    const std::string m_font_awesome_path = "assets/fonts/fa-solid-900.ttf";
  };
}
}
