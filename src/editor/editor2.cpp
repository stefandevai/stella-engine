#include "editor/editor2.hpp"
#include "editor/icons.hpp"
#include "editor/actions.hpp"
#include "stella/game.hpp"
#include "stella/graphics/framebuffer.hpp"
#include "stella/graphics/opengl.hpp"

#include "../../lib/imgui/imgui.h"                       // IWYU pragma: export
#include "../../lib/imgui/backends/imgui_impl_opengl3.h" // IWYU pragma: export
#include "../../lib/imgui/backends/imgui_impl_sdl.h"     // IWYU pragma: export
#include "imgui_internal.h"

#include <stdexcept>
#include <spdlog/spdlog.h>
#include <filesystem>

namespace editor
{
Editor::Editor (const std::string& path) : m_config_path (path)
{
  spdlog::set_level (spdlog::level::debug);
  m_init();
}

Editor::~Editor() { m_deinit_imgui(); }

void Editor::run()
{
  while (m_game->m_display.is_running())
  {
    switch (m_current_mode)
    {
      case EditorMode::NONE:
      {
        m_run_none_mode();
      }
      break;

      case EditorMode::EDIT:
      {
        m_run_edit_mode();
      }
      break;

      case EditorMode::PLAY:
      {
        m_run_play_mode();
      }
      break;

      default:
        break;
    }
  }
}

void Editor::m_run_none_mode()
{
  m_game->m_display.update();
  m_handle_none_mode_input();
  m_handle_none_mode_actions();
  m_game->m_display.clear();
  m_render_none_mode();
}

void Editor::m_run_edit_mode()
{
  // TODO: Save current state to restore when getting back from play mode
  ImGuiIO& io = ImGui::GetIO();
  m_handle_edit_mode_tool (io);
  m_game->m_display.update();
  m_game->update (m_game->m_display.get_dt());
  m_handle_edit_mode_input();
  m_handle_edit_mode_actions();

  m_FBO->bind();
  m_game->m_display.clear();
  m_game->render (m_game->m_display.get_dt());
  m_FBO->unbind();

  m_render_edit_mode (m_game->m_display.get_window_width(), m_game->m_display.get_window_height(), m_game->m_display.m_width, m_game->m_display.m_height);
  m_game->m_display.render();
}

void Editor::m_run_play_mode()
{
  ImGuiIO& io = ImGui::GetIO();
  m_handle_play_mode_tool (io);
  m_game->m_display.update();
  m_game->update (m_game->m_display.get_dt());
  m_handle_play_mode_input();
  m_handle_play_mode_actions();

  m_game->m_display.clear();
  m_game->render (m_game->m_display.get_dt());

  m_render_play_mode (m_game->m_display.get_window_width(), m_game->m_display.get_window_height(), m_game->m_display.m_width, m_game->m_display.m_height);
  m_game->m_display.render();
}

void Editor::m_render_none_mode()
{
  if (m_window == nullptr)
  {
    throw std::runtime_error ("[x] Window is invalid");
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame (m_window);
  ImGui::NewFrame();
  ImGui::PushFont (m_font_sans_regular);

  m_render_menu_bar();

  ImGui::PopFont();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}

void Editor::m_render_edit_mode (const float window_width, const float window_height, const float game_width, const float game_height)
{
  m_window_width  = window_width;
  m_window_height = window_height;
  m_game_width    = game_width;
  m_game_height   = game_height;

  if (m_window == nullptr)
  {
    throw std::runtime_error ("[x] Window is invalid");
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame (m_window);
  ImGui::NewFrame();
  ImGui::PushFont (m_font_sans_regular);

  m_render_menu_bar();

  const auto last_mode = m_current_mode;
  m_toolbar.render (m_game->m_registry, m_current_mode, m_current_tool, m_window_width);
  // TODO: Refactor toolbar code
  // Check if tool has change
  if (m_current_mode != last_mode)
  {
    if (m_current_mode == EditorMode::PLAY)
    {
      m_game->m_display.update_viewport();
    }
  }

  m_render_dock();

  // If the current scene was modified, change the title
  if (m_game->m_current_scene != nullptr)
  {
    if (m_game->m_current_scene->is_modified() != m_scene.has_modify_indication())
    {
      m_scene.set_modify_indication (m_game->m_current_scene->is_modified());
    }

    m_scene.render ((void*) (intptr_t) m_FBO->get_texture());
  }

  // m_inspector.render (m_game->m_registry, m_game->m_textures.get_list());
  m_scene_editor.render (m_game);
  // m_console.render();
  m_assets.render (m_game->m_asset_manager);

  ImGui::PopFont();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}

void Editor::m_render_play_mode (const float window_width, const float window_height, const float game_width, const float game_height)
{
  m_window_width  = window_width;
  m_window_height = window_height;
  m_game_width    = game_width;
  m_game_height   = game_height;

  if (m_window == nullptr)
  {
    throw std::runtime_error ("[x] Window is invalid");
  }

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame (m_window);
  ImGui::NewFrame();
  ImGui::PushFont (m_font_sans_regular);

  m_render_menu_bar();
  m_toolbar.render (m_game->m_registry, m_current_mode, m_current_tool, m_window_width);

  m_render_game_info();

  ImGui::PopFont();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
}

void Editor::m_render_game_info()
{
  static float fps = 0.0f;
  static float dt  = 0.0f;

  // Update values
  if (ImGui::GetFrameCount() % 30 == 0)
  {
    fps = m_game->m_display.get_fps();
    dt  = m_game->m_display.get_dt();
  }

  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
  ImGui::SetNextWindowBgAlpha (0.35f);
  ImGui::SetNextWindowPos (ImVec2 (15.0f, 72.0f), ImGuiCond_Always);
  if (ImGui::Begin ("Example: Simple overlay", nullptr, window_flags))
  {
    ImGui::Text ("Game Info");
    ImGui::Separator();
    ImGui::Text ("FPS: %.5f", fps);
    ImGui::Text ("Frame: %.5f", dt);
  }
  ImGui::End();
}

void Editor::m_init()
{
  m_game = std::make_shared<stella::Game> (m_config_path);

  // Set initial Scene Viewer title
  if (m_game->m_current_scene != nullptr)
  {
    m_scene.set_title ("Scene: " + m_game->m_current_scene->get_name());
  }

  m_window       = m_game->m_display.m_window;
  m_FBO          = std::make_unique<stella::graphics::Framebuffer> (m_game->m_display);
  m_current_mode = EditorMode::EDIT;
  m_init_imgui();
}

void Editor::m_init_imgui()
{
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io    = ImGui::GetIO();
  io.IniFilename = m_imgui_ini_path.c_str();

  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  ImFontConfig config;
  config.OversampleH         = 2;
  config.OversampleV         = 1;
  config.GlyphExtraSpacing.x = 0.5f;

  m_font_sans_bold = io.Fonts->AddFontFromFileTTF (m_font_sans_bold_path.c_str(), 13.0f, &config);
  m_font_mono      = io.Fonts->AddFontFromFileTTF (m_font_mono_path.c_str(), 14.0f);

  // Merge font awesome font with Lato-Regular
  m_font_sans_regular                = io.Fonts->AddFontFromFileTTF (m_font_sans_regular_path.c_str(), 13.0f, &config);
  config.MergeMode                   = true;
  config.GlyphMinAdvanceX            = 13.0f;
  static const ImWchar icon_ranges[] = {ICON_FA_MIN, ICON_FA_MAX, 0};
  io.Fonts->AddFontFromFileTTF (m_font_awesome_path.c_str(), 13.0f, &config, icon_ranges);

  ImGuiStyle& style       = ImGui::GetStyle();
  style.WindowPadding     = ImVec2 (18.0f, 12.0f);
  style.WindowBorderSize  = 0.f;
  style.WindowRounding    = 0.0f;
  style.ScrollbarSize     = 11.0f;
  style.ScrollbarRounding = 10.0f;
  style.TabRounding       = 0.f;
  style.TabBorderSize     = 0.f;

  style.Colors[ImGuiCol_Text]           = ImVec4 (1.00f, 1.00f, 1.00f, 1.00f);
  style.Colors[ImGuiCol_TextDisabled]   = ImVec4 (0.50f, 0.50f, 0.50f, 1.00f);
  style.Colors[ImGuiCol_WindowBg]       = ImVec4 (0.06f, 0.06f, 0.06f, 0.94f);
  style.Colors[ImGuiCol_ChildBg]        = ImVec4 (1.00f, 1.00f, 1.00f, 0.00f);
  style.Colors[ImGuiCol_PopupBg]        = ImVec4 (0.08f, 0.08f, 0.08f, 0.94f);
  style.Colors[ImGuiCol_Border]         = ImVec4 (0.43f, 0.43f, 0.50f, 0.50f);
  style.Colors[ImGuiCol_BorderShadow]   = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
  style.Colors[ImGuiCol_FrameBg]        = ImVec4 (0.20f, 0.21f, 0.22f, 0.54f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4 (0.40f, 0.40f, 0.40f, 0.40f);
  style.Colors[ImGuiCol_FrameBgActive]  = ImVec4 (0.18f, 0.18f, 0.18f, 0.67f);
  style.Colors[ImGuiCol_TitleBg]        = ImVec4 (0.04f, 0.04f, 0.04f, 1.00f);
  style.Colors[ImGuiCol_TitleBgActive]  = ImVec4 (0.04f, 0.04f, 0.04f, 1.00f);
  // style.Colors[ImGuiCol_TitleBgActive]         = ImVec4 (0.29f, 0.29f, 0.29f, 1.00f);
  style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4 (0.00f, 0.00f, 0.00f, 0.51f);
  style.Colors[ImGuiCol_MenuBarBg]             = ImVec4 (0.14f, 0.14f, 0.14f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4 (0.02f, 0.02f, 0.02f, 0.53f);
  style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4 (0.31f, 0.31f, 0.31f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4 (0.41f, 0.41f, 0.41f, 1.00f);
  style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4 (0.51f, 0.51f, 0.51f, 1.00f);
  style.Colors[ImGuiCol_CheckMark]             = ImVec4 (0.94f, 0.94f, 0.94f, 1.00f);
  style.Colors[ImGuiCol_SliderGrab]            = ImVec4 (0.51f, 0.51f, 0.51f, 1.00f);
  style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4 (0.86f, 0.86f, 0.86f, 1.00f);
  style.Colors[ImGuiCol_Button]                = ImVec4 (0.44f, 0.44f, 0.44f, 0.40f);
  style.Colors[ImGuiCol_ButtonHovered]         = ImVec4 (0.46f, 0.47f, 0.48f, 1.00f);
  style.Colors[ImGuiCol_ButtonActive]          = ImVec4 (0.42f, 0.42f, 0.42f, 1.00f);
  style.Colors[ImGuiCol_Header]                = ImVec4 (0.70f, 0.70f, 0.70f, 0.31f);
  style.Colors[ImGuiCol_HeaderHovered]         = ImVec4 (0.70f, 0.70f, 0.70f, 0.80f);
  style.Colors[ImGuiCol_HeaderActive]          = ImVec4 (0.48f, 0.50f, 0.52f, 1.00f);
  style.Colors[ImGuiCol_Separator]             = ImVec4 (0.43f, 0.43f, 0.50f, 0.50f);
  style.Colors[ImGuiCol_SeparatorHovered]      = ImVec4 (0.72f, 0.72f, 0.72f, 0.78f);
  style.Colors[ImGuiCol_SeparatorActive]       = ImVec4 (0.51f, 0.51f, 0.51f, 1.00f);
  style.Colors[ImGuiCol_ResizeGrip]            = ImVec4 (0.91f, 0.91f, 0.91f, 0.25f);
  style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4 (0.81f, 0.81f, 0.81f, 0.67f);
  style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4 (0.46f, 0.46f, 0.46f, 0.95f);
  style.Colors[ImGuiCol_PlotLines]             = ImVec4 (0.61f, 0.61f, 0.61f, 1.00f);
  style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4 (1.00f, 0.43f, 0.35f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogram]         = ImVec4 (0.73f, 0.60f, 0.15f, 1.00f);
  style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4 (1.00f, 0.60f, 0.00f, 1.00f);
  style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4 (0.87f, 0.87f, 0.87f, 0.35f);
  style.Colors[ImGuiCol_ModalWindowDimBg]      = ImVec4 (0.80f, 0.80f, 0.80f, 0.35f);
  style.Colors[ImGuiCol_DragDropTarget]        = ImVec4 (1.00f, 1.00f, 0.00f, 0.90f);
  style.Colors[ImGuiCol_NavHighlight]          = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
  style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4 (1.00f, 1.00f, 1.00f, 0.70f);

  ImVec4 tab_color (60.f / 255.f, 32.f / 255.f, 84.f / 255.f, 1.00f);
  style.Colors[ImGuiCol_Tab]                = tab_color;
  style.Colors[ImGuiCol_TabActive]          = tab_color;
  style.Colors[ImGuiCol_TabHovered]         = ImVec4 (80.f / 255.f, 41.f / 255.f, 115.f / 255.f, 1.00f);
  style.Colors[ImGuiCol_TabUnfocused]       = tab_color;
  style.Colors[ImGuiCol_TabUnfocusedActive] = tab_color;

  ImGui_ImplSDL2_InitForOpenGL (m_window, m_game->m_display.m_gl_context);
  ImGui_ImplOpenGL3_Init (m_game->m_display.m_glsl_version);
}

void Editor::m_deinit_imgui()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}

void Editor::m_handle_none_mode_input()
{
  ImGui_ImplSDL2_ProcessEvent (&m_game->m_display.m_event);
  // const Uint8* state = SDL_GetKeyboardState (nullptr);
}

void Editor::m_handle_edit_mode_input()
{
  ImGui_ImplSDL2_ProcessEvent (&m_game->m_display.m_event);
  const Uint8* state = SDL_GetKeyboardState (nullptr);

  // TODO: replace with a map of shortcuts
  // Save game as
  if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_S])
  {
    m_current_action = Action::SAVE_GAME_AS;
  }
  // Save game
  else if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
  {
    m_current_action = Action::SAVE_GAME;
  }
  // Pan tool
  else if (state[SDL_SCANCODE_SPACE] && ImGui::IsMouseDragging (0))
  {
    // For the first time using the pan tool, save the camera position
    if (m_current_tool != EditorTool::PAN)
    {
      m_game->m_current_scene->m_camera.save_position();
      m_current_tool = EditorTool::PAN;
    }
  }
}

void Editor::m_handle_play_mode_input()
{
  ImGui_ImplSDL2_ProcessEvent (&m_game->m_display.m_event);
  const Uint8* state = SDL_GetKeyboardState (nullptr);

  // TODO: replace with a map of shortcuts
  // Save game as
  if (state[SDL_SCANCODE_ESCAPE])
  {
    m_current_action = Action::QUIT_PLAY_MODE;
  }
}

void Editor::m_handle_edit_mode_tool (ImGuiIO& io)
{
  switch (m_current_tool)
  {
    case EditorTool::TILE_PEN:
    {
      // this->m_handle_tile_pen (io);
    }
    break;
    case EditorTool::INSPECTOR:
    {
      // this->m_handle_inspector (io);
    }
    break;
    case EditorTool::PAN:
    {
      m_handle_pan_tool();
    }
    break;
    default:
      break;
  }
}

void Editor::m_handle_play_mode_tool (ImGuiIO& io)
{
  switch (m_current_tool)
  {
    default:
      break;
  }
}

void Editor::m_handle_none_mode_actions()
{
  switch (m_current_action)
  {
    case Action::QUIT_EDITOR:
    {
      m_game->quit();
    }
    break;

    default:
      break;
  }
}

void Editor::m_handle_edit_mode_actions()
{
  switch (m_current_action)
  {
    case Action::QUIT_EDITOR:
    {
      m_game->quit();
    }
    break;

    case Action::SAVE_GAME:
    {
      m_game->save();
    }
    break;

    case Action::NEW_SCENE:
    {
      m_new_scene_popup.open();
    }
    break;

    case Action::LOAD_SCENE:
    {
      m_load_scene_popup.open();
    }
    break;

    case Action::SAVE_SCENE:
    {
      if (m_game->m_current_scene != nullptr)
      {
        m_game->m_current_scene->save();
      }
    }
    break;

    default:
      break;
  }

  m_current_action = Action::NONE;
}

void Editor::m_handle_play_mode_actions()
{
  switch (m_current_action)
  {
    case Action::QUIT_EDITOR:
    {
      m_game->quit();
    }
    break;

    case Action::QUIT_PLAY_MODE:
    {
      m_game->m_display.reset_viewport();
      m_current_mode = EditorMode::EDIT;
    }
    break;

    default:
      break;
  }

  m_current_action = Action::NONE;
}

void Editor::m_handle_pan_tool()
{
  ImVec2 drag = ImGui::GetMouseDragDelta();

  // Break after no more drag
  if (drag.x == 0.0f && drag.y == 0.0f)
  {
    // TODO: use a stack of tools
    m_current_tool = EditorTool::INSPECTOR;
    return;
  }

  const float drag_factor           = 1.0f;
  const auto& camera_saved_position = m_game->m_current_scene->m_camera.get_saved_position();
  m_game->m_current_scene->m_camera.set_position (camera_saved_position.x - drag.x * drag_factor, camera_saved_position.y - drag.y * drag_factor, camera_saved_position.z);
}

// TODO: Render a diferent menu bar for each mode
void Editor::m_render_menu_bar()
{
  m_current_action = m_edit_mode_main_menu_options.render();

  m_new_scene_popup.render();
  m_load_scene_popup.render();

  // If a new scene was created, loaded or started
  if (m_game != nullptr && m_game->m_current_scene != nullptr && m_scene.get_title() != ("Scene: " + m_game->m_current_scene->get_name()))
  {
    m_scene.set_title ("Scene: " + m_game->m_current_scene->get_name());
    m_scene_editor.reload();
  }
}

void Editor::m_render_dock()
{
  float dock_width  = m_window_width;
  float dock_height = m_window_height;
  float dock_offset = 0.f;

  // Adjust dock size if toolbar is open
  if (m_toolbar.is_open())
  {
    const ImVec2& toolbar_size = m_toolbar.size();
    dock_height -= toolbar_size.y;
    dock_offset = toolbar_size.y;
  }

  ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar (ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));

  ImGui::SetNextWindowSize (ImVec2 (dock_width, dock_height), ImGuiCond_Always);
  ImGui::SetNextWindowPos (ImVec2 (0, dock_offset), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha (0.0f);
  ImGui::Begin ("MainDS",
                nullptr,
                ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                    ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
  ImGui::PopStyleVar();
  ImGui::PopStyleVar (2);

  const auto dockspace_id = ImGui::GetID ("MainDS");
  if (!ImGui::DockBuilderGetNode (dockspace_id))
  {
    ImGui::DockBuilderRemoveNode (dockspace_id);
    ImGui::DockBuilderAddNode (dockspace_id, ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize (dockspace_id, ImVec2 (m_window_width, m_window_height));

    ImGuiID dock_main_id  = dockspace_id;
    ImGuiID dock_right_id = ImGui::DockBuilderSplitNode (dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
    // ImGuiID dock_right_down_id =
    // ImGui::DockBuilderSplitNode (dock_right_id, ImGuiDir_Down, 0.5f, nullptr, &dock_right_id);
    ImGuiID dock_down_id = ImGui::DockBuilderSplitNode (dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

    ImGui::DockBuilderDockWindow (m_scene_editor.get_title_string().c_str(), dock_right_id);
    // ImGui::DockBuilderDockWindow (m_inspector.get_title_string().c_str(), dock_right_down_id);
    ImGui::DockBuilderDockWindow (m_assets.get_title_string().c_str(), dock_down_id);
    ImGui::DockBuilderDockWindow (m_scene.get_title_string().c_str(), dock_main_id);

    ImGui::DockBuilderFinish (dock_main_id);
  }

  ImGui::DockSpace (dockspace_id, ImVec2 (0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::End();
}

void Editor::m_render_view_menu_option (widget::Widget& widget, const std::string& shortcut)
{
  auto item_text = widget.get_title_string();
  if (widget.is_open())
  {
    item_text = "Hide " + widget.get_title_string();
  }
  if (ImGui::MenuItem (item_text.c_str(), shortcut.c_str()))
  {
    widget.toggle();
  }
}
} // namespace editor
