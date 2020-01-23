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
    m_editor_layer = game.m_registry.create();
    game.m_registry.assign<components::LayerComponent>(m_editor_layer, "editor", 9999, "", "", "");

    ImVec2 dimensions = m_tileset_editor.get_tile_dimensions();
    m_editor_sprite = game.m_registry.create();
    game.m_registry.assign<components::PositionComponent>(m_editor_sprite, 100, 100);
    game.m_registry.assign<components::DimensionComponent>(m_editor_sprite, dimensions.x, dimensions.y);
    game.m_registry.assign<components::SpriteComponent>(m_editor_sprite,
                                                        m_tileset_editor.texture,
                                                        m_tileset_editor.get_tile_dimensions().x,
                                                        m_tileset_editor.get_tile_dimensions().y,
                                                        36,
                                                        "editor");
  }

  EditorGui::~EditorGui() { }

  void EditorGui::init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version)
  {
    m_window = window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImFontConfig config;
    config.OversampleH = 2;
    config.OversampleV = 1;
    config.GlyphExtraSpacing.x = 0.5f;

    m_font_sans_bold = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Bold.ttf", 13.0f, &config);
    m_font_mono = io.Fonts->AddFontFromFileTTF("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 14.0f);
    
    // Merge font awesome font with Lato-Regular
    m_font_sans_regular = io.Fonts->AddFontFromFileTTF("assets/fonts/Lato/Lato-Regular.ttf", 13.0f, &config);
    config.MergeMode = true;
    config.GlyphMinAdvanceX = 13.0f;
    static const ImWchar icon_ranges[] = { ICON_FA_MIN, ICON_FA_MAX, 0 };
    io.Fonts->AddFontFromFileTTF("assets/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

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
      m_map_editor.update_map_settings();
      m_game.m_tile_map.save(m_map_editor.get_map_path());
      m_log.AddLog("Saved map...\n");
    }

    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_O])
    {
      m_log.AddLog("Loading map...\n");
      m_game.m_tile_map.load(m_map_editor.get_map_path());
      m_map_editor.reset_map_settings();
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
      ImGui::PushFont(m_font_sans_regular);

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

      ImGui::PopFont();
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
            this->handle_tile_pen(io);
            break;
          case INSPECTOR:
            this->handle_inspector(io);
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

  void EditorGui::handle_tile_pen(ImGuiIO& io)
  {
    // If the mouse is within the game screen boundaries
            if (io.MousePos.x > (m_window_width - m_game_width) &&
                io.MousePos.x < (m_window_width) &&
                io.MousePos.y < (m_game_height + 23) &&
                io.MousePos.y > 23)
            {
              // Set dummy sprite position with grid snapping
              const auto& camera_pos = m_game.get_camera_pos();
              float width_padding = m_window_width - m_game_width;
              float height_padding = 23.f;

              auto& sprite_pos = m_registry.get<components::PositionComponent>(m_editor_sprite);
              auto& sprite_spr = m_registry.get<components::SpriteComponent>(m_editor_sprite);
              ImVec2 tile_pos = m_tileset_editor.pos2tile(io.MousePos.x - width_padding + camera_pos[0], io.MousePos.y - height_padding + camera_pos[1]);
              int new_tile_value = m_tileset_editor.get_selected_tile_id();

              sprite_pos.x = tile_pos.x*m_tileset_editor.get_tile_dimensions().x;
              sprite_pos.y = tile_pos.y*m_tileset_editor.get_tile_dimensions().y;
              sprite_spr.Sprite->SetDirectFrame(new_tile_value);

                if (io.MouseClicked[0])
                {
                  // Update tile if user clicks
                  bool collidable = m_tileset_editor.get_selected_tile_collidable();
                  int layer_id = m_map_editor.get_selected_layer_id();
                  
                  m_game.m_tile_map.update_tile(new_tile_value, tile_pos.x, tile_pos.y, layer_id, collidable);
                }
            }
  }

  void EditorGui::handle_inspector(ImGuiIO& io)
  {
    std::cout << "Inspector\n";
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
    
    this->draw_toolbar();
    m_inspector.render();
    m_map_editor.render();
    m_tileset_editor.render();
    //this->draw_log();
    //ImGui::Text("Add tool panels here.");
    ImGui::End();
  }

  void EditorGui::draw_toolbar()
  {
    ImGui::PushFont(m_font_sans_regular);

    ImGui::PushID("inspector-button");
    if (ImGui::Button(ICON_FA_MOUSE_POINTER))
    {
      if (m_current_state != EDIT)
      {
        m_current_state = EDIT;
      }
      m_current_tool = INSPECTOR;
    }
    ImGui::PopID();
    ImGui::SameLine();

    ImGui::PushID("tile-pen-button");
    if (ImGui::Button(ICON_FA_EDIT))
    {
      if (m_current_state != EDIT)
      {
        m_current_state = EDIT;
      }
      m_current_tool = TILE_PEN;
    }

    ImGui::SameLine();
    ImGui::PopID();

    ImGui::PushID("play-button");
    if(ImGui::Button(ICON_FA_PLAY))
    {
      m_current_state = PLAY;
    }
    ImGui::PopID();

    ImGui::PopFont();
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

