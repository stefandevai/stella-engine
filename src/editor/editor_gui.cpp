#include "editor/editor_gui.h"

#include "stella/components/layer_component.h"
#include "stella/components/sprite_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "../../nikte/game.h"

#include "editor/debug_layer.h"

#include <cereal/cereal.hpp>
#include <SDL2/SDL.h>

namespace stella
{
namespace editor
{

  // EditorGui::EditorGui(entt::registry& registry)
  //   : m_registry(registry)
  EditorGui::EditorGui(nikte::Game& game)
    : m_game(game), m_registry(game.m_registry)
  {
    m_game.m_display.SetEditor(this);
    //m_debug_layer.Add(shape);
  }

  EditorGui::~EditorGui() { }

  void EditorGui::init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version)
  {
    m_window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 1;
    config.GlyphExtraSpacing.x = 0.5f;

    m_font_sans_regular = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Regular.ttf", 16.0f, &config);
    m_font_sans_bold = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Bold.ttf", 16.0f, &config);
    //m_font_sans_regular = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 13.0f);
    //m_font_sans_bold = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 13.0f);
    m_font_mono = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 13.0f);

    this->init_style();

    ImGui_ImplSDL2_InitForOpenGL(m_window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);
  }

  void EditorGui::configure_input(SDL_Event &event)
  {
    ImGui_ImplSDL2_ProcessEvent(&event);
    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    // Save game
    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
    { 
      m_log.AddLog("Saving map...\n");
      m_game.m_tile_map.save("./editor-map.xml");
      m_log.AddLog("Saved map...\n");
    }

    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_O])
    {
      m_log.AddLog("Loading map...\n");
      m_game.m_tile_map.load("./editor-map.xml");
      m_log.AddLog("Loaded map...\n");
    }
  }

  void EditorGui::update()
  {
    m_log_system.update(m_registry, 0.0);
  }

  void EditorGui::render(const float window_width, const float window_height, const float game_width, const float game_height)
  {
    if (m_window != nullptr)
    {
      const float section_spacing = 1.0f;
      const float top_menu_height = 22.0f + section_spacing;

      m_window_width = window_width;
      m_window_height = window_height;
      m_game_width = game_width;
      m_game_height = game_height;

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame(m_window);
      ImGui::NewFrame();

      ImGuiIO& io = ImGui::GetIO();
      handle_state(io);

      const ImVec2 editor_size{window_width - game_width - section_spacing, window_height - top_menu_height};
      const ImVec2 editor_pos{0.0f, top_menu_height};
      const ImVec2 console_size{game_width, window_height - game_height - top_menu_height - section_spacing};
      const ImVec2 console_pos{window_width - game_width, game_height + top_menu_height + section_spacing};
      const ImVec2 log_size{window_width - game_width, window_height - game_height - top_menu_height - section_spacing};
      const ImVec2 log_pos{0.0f, game_height + top_menu_height + section_spacing};
      const ImVec2 info_pos{window_width - 148.f - top_menu_height, top_menu_height*2};
      
      this->draw_editor(editor_size, editor_pos);
      this->draw_console(console_size, console_pos);
      this->draw_menu_bar();

      if (m_view_physics_debug_layer)
      {
        this->draw_info(info_pos);
        //m_debug_layer.Render();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
  }

  void EditorGui::clean()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void EditorGui::handle_state(ImGuiIO& io)
  {
    switch(m_current_state)
    {
      case EDIT:
        switch (m_current_tool)
        {
          case TILE_PEN:
            if (io.MousePos.x > (m_window_width - m_game_width) &&
                io.MousePos.x < (m_window_width) &&
                io.MousePos.y < (m_game_height + 23) &&
                io.MousePos.y > 23)
            {
              const auto& camera_pos = m_game.get_camera_pos();
              ImVec2 tile_pos = m_tileset_editor.pos2tile(camera_pos[0] - m_game_width/2 + io.MousePos.x, camera_pos[1] + io.MousePos.y);
              
              m_tileset_editor.render_tile_sprite(ImVec2(io.MousePos.x - m_tileset_editor.get_tile_dimensions().x, io.MousePos.y - m_tileset_editor.get_tile_dimensions().y), 0.6f);
              //m_tileset_editor.render_tile_sprite(ImVec2((tile_pos.x-1), tile_pos.y));
                if (io.MouseClicked[0])
                {
                  int new_tile_value = m_tileset_editor.get_selected_tile_id();

                  // TODO: Change way of handling layers
                  (*m_game.m_tile_map.tile_layers.begin())->set_value(tile_pos.x-1, tile_pos.y-1, new_tile_value);
                }
            }
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

  void EditorGui::init_style()
  {
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(18.0f, 12.0f);
    style.WindowBorderSize = 0.f;
    style.WindowRounding = 3.0f;
    style.ScrollbarSize = 11.0f;
    style.ScrollbarRounding = 10.0f;

    style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  }

  void EditorGui::draw_editor(const ImVec2 &size, const ImVec2 &pos)
  {
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::Begin("Editor", nullptr, m_window_flags);
    
    m_inspector.render();
    m_tileset_editor.render();
    m_layer_editor.render();
    //this->draw_log();
    //ImGui::Text("Add tool panels here.");
    ImGui::End();
  }

  void EditorGui::draw_console(const ImVec2 &size, const ImVec2 &pos)
  {
    ImGui::SetNextWindowSize(size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f,0.5f,0.5f,1.0f));
    //ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
    m_console.Draw("Console Log", m_registry);
    //ImGui::PopStyleVar();
    ImGui::PopStyleColor();
  }

  void EditorGui::draw_log()
  {
    m_log.Draw("Console Log");
  }

  void EditorGui::draw_info(const ImVec2 &pos)
  {
    ImGui::SetNextWindowBgAlpha(0.4);
    ImGui::SetNextWindowPos(pos, ImGuiCond_Always);
    ImGui::Begin("Info", nullptr, m_window_flags | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushFont(m_font_sans_bold);
    ImGui::Text("  - Stella Engine -  ");
    ImGui::PopFont();
    ImGui::Dummy(ImVec2(0.0f, 3.0f));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0.0f, 6.0f));
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Dummy(ImVec2(0.0f, 3.0f));
    ImGui::End();
  }

  void EditorGui::draw_menu_bar()
  {
    if (ImGui::BeginMainMenuBar())
    {
      if (ImGui::BeginMenu("File"))
      {
        if (ImGui::MenuItem("New Tilemap", "CTRL+N")) {}
        if (ImGui::MenuItem("Open Tilemap", "CTRL+O")) {}
        if (ImGui::MenuItem("Open Tileset", "SHIFT+O")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Save", "CTRL+S")) {}
        if (ImGui::MenuItem("Save as...", "CTRL+SHIFT+S")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "CTRL+W")) {}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Edit"))
      {
        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
        ImGui::Separator();
        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("View"))
      {
        auto item_text = "View Physics debug layer";
        if (m_view_physics_debug_layer) item_text = "Hide Physics debug layer";
        if (ImGui::MenuItem(item_text, "CTRL+D"))
        {
          m_view_physics_debug_layer = !m_view_physics_debug_layer;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu("Tools"))
      {
          if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
          ImGui::Separator();
          if (ImGui::MenuItem("Paste", "CTRL+V")) {}
          ImGui::EndMenu();
      }
      ImGui::EndMainMenuBar();
    }
  }
}
}

