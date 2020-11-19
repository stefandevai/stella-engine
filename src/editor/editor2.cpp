#include "editor/editor2.hpp"
#include "editor/icons.hpp"
#include "stella/game2.hpp"
#include "stella/graphics/framebuffer.hpp"

#include "../../lib/imgui/imgui.h"                       // IWYU pragma: export
#include "../../lib/imgui/examples/imgui_impl_opengl3.h" // IWYU pragma: export
#include "../../lib/imgui/examples/imgui_impl_sdl.h"     // IWYU pragma: export
#include "imgui_internal.h"

// TEMP
#include <iostream>
// TEMP

namespace stella
{
namespace editor
{
  Editor::Editor (stella::Game& game)
    : m_game(game)
  {
    m_init();
  }

  Editor::~Editor()
  {
    m_deinit_imgui();
  }

  void Editor::render (const float window_width, const float window_height, const float game_width, const float game_height)
  {
    m_window_width  = window_width;
    m_window_height = window_height;
    m_game_width    = game_width;
    m_game_height   = game_height;

    if (m_window != nullptr)
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame (m_window);
      ImGui::NewFrame();
      ImGui::PushFont (m_font_sans_regular);

      m_render_menu_bar();
      m_toolbar.render (m_game.m_registry, m_current_state, m_current_tool, m_window_width);

      if (m_current_state == EDIT)
      {
        m_render_dock();

        m_scene.render ((void*) (intptr_t) m_FBO->get_texture());
        m_inspector.render (m_game.m_registry, m_game.m_textures.get_list());
        m_scene_editor.render (m_game.m_current_scene);
        m_console.render();
      }

      ImGui::PopFont();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
    }
  }

  void Editor::update (const double dt)
  {
    for (auto& s : m_systems)
    {
      s->update (m_registry, dt);
    }
  }

  void Editor::run()
  {
    while (m_game.m_display.is_running())
    {
      m_game.update (m_game.m_display.get_dt());
      m_game.m_display.update();
      m_handle_input();
      update (m_game.m_display.get_dt());

      m_FBO->bind();
      m_game.m_display.clear();
      m_game.render (m_game.m_display.get_dt());
      m_FBO->unbind();
      render (m_game.m_display.get_window_width(),
              m_game.m_display.get_window_height(),
              m_game.m_display.m_width,
              m_game.m_display.m_height);
    }
  }

  void Editor::m_init()
  {
    m_window = m_game.m_display.m_window;
    m_FBO = std::make_unique<graphics::Framebuffer> (m_game.m_display);
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
    m_font_sans_regular     = io.Fonts->AddFontFromFileTTF (m_font_sans_regular_path.c_str(), 13.0f, &config);
    config.MergeMode        = true;
    config.GlyphMinAdvanceX = 13.0f;
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
    style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4 (0.80f, 0.80f, 0.80f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]        = ImVec4 (1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]          = ImVec4 (0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4 (1.00f, 1.00f, 1.00f, 0.70f);

    ImVec4 tab_color (60.f / 255.f, 32.f / 255.f, 84.f / 255.f, 1.00f);
    style.Colors[ImGuiCol_Tab]                = tab_color;
    style.Colors[ImGuiCol_TabActive]          = tab_color;
    style.Colors[ImGuiCol_TabHovered]         = ImVec4 (80.f / 255.f, 41.f / 255.f, 115.f / 255.f, 1.00f);
    style.Colors[ImGuiCol_TabUnfocused]       = tab_color;
    style.Colors[ImGuiCol_TabUnfocusedActive] = tab_color;

    ImGui_ImplSDL2_InitForOpenGL (m_window, m_game.m_display.m_gl_context);
    ImGui_ImplOpenGL3_Init (m_game.m_display.m_glsl_version);
  }

  void Editor::m_deinit_imgui()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void Editor::m_handle_input()
  {
    ImGui_ImplSDL2_ProcessEvent (&m_game.m_display.m_event);
    const Uint8* state = SDL_GetKeyboardState (nullptr);

    // TODO: replace with a map of shortcuts
    switch (m_current_state)
    {
      case EDIT:
      {
        // Save game as
        if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_S])
        {
        }
        // Save game
        else if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
        {
        }
      }
      break;

      case PLAY:
      {
      }
      break;
      default:
        break;
    }
  }

  void Editor::m_handle_state (ImGuiIO& io)
  {
    switch (m_current_state)
    {
      case EDIT:
        switch (m_current_tool)
        {
          case TILE_PEN:
            //this->m_handle_tile_pen (io);
            break;
          case INSPECTOR:
            //this->m_handle_inspector (io);
            break;
          case PAN:
            //this->m_handle_pan_tool (io);
            break;
          default:
            break;
        }
        break;
      case PLAY:
        break;
      default:
        break;
    }
  }

  void Editor::m_render_menu_bar()
  {
    // TODO: Use a enum for menu actions
    std::string menu_action = "";

    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu ("File"))
      {
        if (ImGui::MenuItem ("New Game", "CTRL+N"))
        {
          menu_action = "new_game";
        }

        if (ImGui::MenuItem ("Load Game", "CTRL+N"))
        {
          menu_action = "load_game";
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Save Game", "CTRL+S"))
        {
          menu_action = "load_game";
        }

        if (ImGui::MenuItem ("Save Game as...", "CTRL+SHIFT+S"))
        {
          menu_action = "load_game";
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Quit", "CTRL+Q"))
        {
          menu_action = "quit_editor";
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("Scene"))
      {
        if (ImGui::MenuItem ("New Scene", "CTRL+N"))
        {
          menu_action = "new_scene";
        }

        if (ImGui::MenuItem ("Load Scene", "CTRL+S"))
        {
          menu_action = "load_scene";
        }

        ImGui::Separator();

        if (ImGui::MenuItem ("Save Scene", "CTRL+S"))
        {
          menu_action = "save_scene";
        }

        if (ImGui::MenuItem ("Save Scene as...", "CTRL+SHIFT+S"))
        {
          menu_action = "save_scene_as";
        }

        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("View"))
      {
        ImGui::Dummy (ImVec2{0.0f, 1.5f});
        m_render_view_menu_option (m_inspector, "oi/ci");
        ImGui::Dummy (ImVec2{0.0f, 3.0f});
        m_render_view_menu_option (m_console, "oy/cy");
        ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }

    if (menu_action == "new_scene")
    {
      m_new_scene_popup.open();
    }
    else if (menu_action == "load_scene")
    {
      m_load_scene_popup.open();
    }
    else if (menu_action == "quit_editor")
    {
      m_game.m_display.m_running = false;
    }

    const bool created_scene = m_new_scene_popup.render();
    const bool loaded_scene = m_load_scene_popup.render();

    // If a new scene was created
    if (created_scene || loaded_scene)
    {
      // Set Scene widget title
      if (m_game.m_current_scene != nullptr)
      {
        m_scene.set_title("Scene: " + m_game.m_current_scene->get_name());
      }

      // Reload scene editor
      m_scene_editor.reload();
    }
  }

  void Editor::m_render_dock()
  {
    ImGuiIO& io = ImGui::GetIO();
    m_handle_state (io);

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
                  ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus |
                      ImGuiWindowFlags_NoNavFocus);
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
      ImGuiID dock_right_down_id =
          ImGui::DockBuilderSplitNode (dock_right_id, ImGuiDir_Down, 0.5f, nullptr, &dock_right_id);
      ImGuiID dock_down_id = ImGui::DockBuilderSplitNode (dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

      ImGui::DockBuilderDockWindow (m_scene_editor.get_title_string().c_str(), dock_right_id);
      ImGui::DockBuilderDockWindow (m_inspector.get_title_string().c_str(), dock_right_down_id);
      ImGui::DockBuilderDockWindow (m_console.get_title_string().c_str(), dock_down_id);
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
}
}
