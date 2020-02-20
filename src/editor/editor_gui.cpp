#include "editor/editor_gui.h"
#include "imgui_internal.h"

#include "../../nikte/game.h"
#include "stella/components/dimension.h"
#include "stella/components/layer.h"
#include "stella/components/position.h"
#include "stella/components/sprite.h"

#include "editor/debug_layer.h"

#include <SDL2/SDL.h>
#undef main
#include <cereal/cereal.hpp>

namespace stella
{
namespace editor
{
  EditorGui::EditorGui (nikte::Game& game) : m_game (game), m_registry (game.m_registry)
  {
    // m_game.m_display.SetEditor (this);
    // m_debug_layer.Add(shape);
    m_editor_layer = game.m_registry.create();
    game.m_registry.assign<component::Layer> (m_editor_layer, "editor", 9999, "", "", "");

    ImVec2 dimensions = m_tileset_editor.get_tile_dimensions();
    m_editor_sprite   = game.m_registry.create();
    game.m_registry.assign<component::Position> (m_editor_sprite, -dimensions.x, -dimensions.y);
    game.m_registry.assign<component::Dimension> (m_editor_sprite, dimensions.x, dimensions.y);
    game.m_registry.assign<component::Sprite> (m_editor_sprite,
                                               m_tileset_editor.texture,
                                               m_tileset_editor.get_tile_dimensions().x,
                                               m_tileset_editor.get_tile_dimensions().y,
                                               0,
                                               "editor");
    this->init();
  }

  EditorGui::~EditorGui()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  // void EditorGui::init (SDL_Window* window, SDL_GLContext gl_context, const char* glsl_version)
  void EditorGui::init()
  {
    m_window = m_game.m_display.Window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImFontConfig config;
    config.OversampleH         = 2;
    config.OversampleV         = 1;
    config.GlyphExtraSpacing.x = 0.5f;

    m_font_sans_bold = io.Fonts->AddFontFromFileTTF ("assets/fonts/Lato/Lato-Bold.ttf", 13.0f, &config);
    m_font_mono      = io.Fonts->AddFontFromFileTTF ("assets/fonts/Ubuntu_Mono/UbuntuMono-Regular.ttf", 14.0f);

    // Merge font awesome font with Lato-Regular
    m_font_sans_regular     = io.Fonts->AddFontFromFileTTF ("assets/fonts/Lato/Lato-Regular.ttf", 13.0f, &config);
    config.MergeMode        = true;
    config.GlyphMinAdvanceX = 13.0f;
    static const ImWchar icon_ranges[] = {ICON_FA_MIN, ICON_FA_MAX, 0};
    io.Fonts->AddFontFromFileTTF ("assets/fonts/fa-solid-900.ttf", 13.0f, &config, icon_ranges);

    this->init_style();

    ImGui_ImplSDL2_InitForOpenGL (m_window, m_game.m_display.m_gl_context);
    ImGui_ImplOpenGL3_Init (m_game.m_display.m_glsl_version);

    m_FBO = std::make_unique<graphics::Framebuffer> (m_game.m_display);
  }

  void EditorGui::configure_input()
  {
    ImGui_ImplSDL2_ProcessEvent (&m_game.m_display.m_event);
    const Uint8* state = SDL_GetKeyboardState (nullptr);

    // Save map
    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
    {
      m_console.add_log ("Saving map...\n");
      m_map_editor.update_map_settings();
      m_game.m_tile_map.save (m_map_editor.get_map_path());
      m_console.add_log ("Saved map...\n");
    }

    // Load map
    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_O])
    {
      m_console.add_log ("Loading map...\n");
      m_game.m_tile_map.load (m_map_editor.get_map_path());
      m_map_editor.reset_map_settings();
      m_console.add_log ("Loaded map...\n");
    }

    // Quit editor
    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_W])
    {
      m_game.m_display.Running = false;
    }

    // Run game without the editor
    if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_R])
    {
      m_show_editor = true;
      glViewport (0, 0, m_game_width, m_game_height);
      
    }
    else if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_R])
    {
      m_show_editor = false;
      glViewport (0, 0, m_game.m_display.GetWindowWidth(), m_game.m_display.GetWindowHeight());
      m_game.m_display.m_check_viewport_proportions();
    }
  }

  void EditorGui::run()
  {
    while (m_game.m_display.IsRunning())
    {
      if (m_show_editor)
      {
        m_FBO->Bind();
        m_game.m_display.Clear();
        m_game.m_systems.front()->update(m_game.m_registry, m_game.m_display.GetDT());
        //m_game.update (m_game.m_display.GetDT());
        m_FBO->Unbind();
        this->render (m_game.m_display.GetWindowWidth(),
                    m_game.m_display.GetWindowHeight(),
                    m_game.m_display.Width,
                    m_game.m_display.Height);

        m_game.m_display.Update();
        this->configure_input();
      }
      else
      {
        m_game.m_display.Clear();
        m_game.update (m_game.m_display.GetDT());
        m_game.m_display.Update();
        this->configure_input();
      }
    }
  }

  void EditorGui::update() { m_log_system.update (m_registry, 0.0); }

  void EditorGui::render (const float window_width,
                          const float window_height,
                          const float game_width,
                          const float game_height)
  {
    if (m_window != nullptr)
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame (m_window);
      ImGui::NewFrame();
      ImGui::PushFont (m_font_sans_regular);

      this->draw_dock (window_width, window_height, game_width, game_height);

      ImGui::PopFont();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
    }
  }

  void EditorGui::handle_state (ImGuiIO& io)
  {
    switch (m_current_state)
    {
      case EDIT:
        switch (m_current_tool)
        {
          case TILE_PEN:
            this->handle_tile_pen (io);
            break;
          case INSPECTOR:
            this->handle_inspector (io);
            break;
          default:
            break;
        }
        break;
      case PLAY:
        m_show_editor = !m_show_editor;
        break;
      default:
        break;
    }
  }

  void EditorGui::handle_tile_pen (ImGuiIO& io)
  {
    // If the mouse is within the game screen boundaries
    if (io.MousePos.x > m_scene.get_x() && io.MousePos.x < m_scene.get_width() &&
        io.MousePos.y < m_scene.get_height() && io.MousePos.y > m_scene.get_y())
    {
      // Set dummy sprite position with grid snapping
      const auto& camera_pos    = m_game.get_camera_pos();
      float width_padding       = m_scene.get_x() + m_scene.get_game_screen_x_spacing();
      float height_padding      = m_scene.get_y() * 2 + m_scene.get_game_screen_y_spacing();
      const float width_factor  = 896 / static_cast<float> (m_scene.get_game_screen_width());
      const float height_factor = 504 / static_cast<float> (m_scene.get_game_screen_height());

      auto& sprite_pos = m_registry.get<component::Position> (m_editor_sprite);
      auto& sprite_spr = m_registry.get<component::Sprite> (m_editor_sprite);

      ImVec2 tile_pos    = m_tileset_editor.pos2tile (io.MousePos.x * width_factor - width_padding + camera_pos[0],
                                                   io.MousePos.y * height_factor - height_padding + camera_pos[1]);
      int new_tile_value = m_tileset_editor.get_selected_tile_id();

      sprite_pos.x = tile_pos.x * m_tileset_editor.get_tile_dimensions().x;
      sprite_pos.y = tile_pos.y * m_tileset_editor.get_tile_dimensions().y;
      sprite_spr.sprite->SetDirectFrame (new_tile_value);

      if (io.MouseClicked[0])
      {
        // Update tile if user clicks
        bool collidable = m_tileset_editor.get_selected_tile_collidable();
        int layer_id    = m_map_editor.get_selected_layer_id();

        m_game.m_tile_map.update_tile (new_tile_value, tile_pos.x, tile_pos.y, layer_id, collidable);
      }
    }
  }

  void EditorGui::handle_inspector (ImGuiIO& io)
  {
    // If the mouse is within the game screen boundaries
    if (io.MousePos.x > m_scene.get_x() && io.MousePos.x < m_scene.get_width() &&
        io.MousePos.y < m_scene.get_height() && io.MousePos.y > m_scene.get_y())
    {
      if (io.MouseClicked[0])
      {
        // Get clicked tile position
        const auto& camera_pos    = m_game.get_camera_pos();
        float width_padding       = m_scene.get_x() + m_scene.get_game_screen_x_spacing();
        float height_padding      = m_scene.get_y() * 2 + m_scene.get_game_screen_y_spacing();
        const float width_factor  = 896 / static_cast<float> (m_scene.get_game_screen_width());
        const float height_factor = 504 / static_cast<float> (m_scene.get_game_screen_height());
        ImVec2 tile_pos = m_tileset_editor.pos2tile (io.MousePos.x * width_factor - width_padding + camera_pos[0],
                                                     io.MousePos.y * height_factor - height_padding + camera_pos[1]);
        auto tile = m_game.m_tile_map.layers[m_map_editor.get_selected_layer_id()]->get_value (tile_pos.x, tile_pos.y);
        m_inspector.set_selected_entity (tile.entity);

        const auto& updated_pos = m_game.m_registry.get<component::Position> (tile.entity);
        tile.x                  = updated_pos.x;
        tile.y                  = updated_pos.y;
        tile.z                  = updated_pos.z;
        m_game.m_tile_map.layers[m_map_editor.get_selected_layer_id()]->set_value (tile_pos.x, tile_pos.y, tile);

        auto tile2 = m_game.m_tile_map.layers[m_map_editor.get_selected_layer_id()]->get_value (tile_pos.x, tile_pos.y);
        m_console.add_log ("%d\n", tile2.z);
      }
    }
  }

  void EditorGui::init_style()
  {
    ImGuiStyle& style       = ImGui::GetStyle();
    style.WindowPadding     = ImVec2 (18.0f, 12.0f);
    style.WindowBorderSize  = 0.f;
    style.WindowRounding    = 3.0f;
    style.ScrollbarSize     = 11.0f;
    style.ScrollbarRounding = 10.0f;
    style.TabRounding       = 0.f;
    style.TabBorderSize     = 0.f;

    style.Colors[ImGuiCol_Text]                  = ImVec4 (1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]          = ImVec4 (0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]              = ImVec4 (0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]               = ImVec4 (1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]               = ImVec4 (0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                = ImVec4 (0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]          = ImVec4 (0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]               = ImVec4 (0.20f, 0.21f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4 (0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]         = ImVec4 (0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]               = ImVec4 (0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]         = ImVec4 (0.29f, 0.29f, 0.29f, 1.00f);
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
    style.Colors[ImGuiCol_TitleBgActive]      = tab_color;
  }

  void EditorGui::draw_dock (const float window_width,
                             const float window_height,
                             const float game_width,
                             const float game_height)
  {
    m_window_width  = window_width;
    m_window_height = window_height;
    m_game_width    = game_width;
    m_game_height   = game_height;

    ImGuiIO& io = ImGui::GetIO();
    handle_state (io);

    ImGui::SetNextWindowSize (ImVec2 (window_width, window_height), ImGuiCond_Always);
    ImGui::SetNextWindowPos (ImVec2 (0, 0), ImGuiCond_Always);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::SetNextWindowBgAlpha (0.0f);
    ImGui::PushStyleVar (ImGuiStyleVar_WindowPadding, ImVec2 (0.0f, 0.0f));
    ImGui::Begin ("MainDS",
                  nullptr,
                  ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
                      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
    ImGui::PopStyleVar();
    ImGui::PopStyleVar (2);

    const auto dockspace_id = ImGui::GetID ("MainDS");
    if (!ImGui::DockBuilderGetNode (dockspace_id))
    {
      ImGui::DockBuilderRemoveNode (dockspace_id);
      ImGui::DockBuilderAddNode (dockspace_id, ImGuiDockNodeFlags_DockSpace);
      ImGui::DockBuilderSetNodeSize (dockspace_id, ImVec2 (window_width, window_height));

      ImGuiID dock_main_id  = dockspace_id;
      ImGuiID dock_right_id = ImGui::DockBuilderSplitNode (dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
      ImGuiID dock_right_down_id =
          ImGui::DockBuilderSplitNode (dock_right_id, ImGuiDir_Down, 0.5f, nullptr, &dock_right_id);
      ImGuiID dock_down_id = ImGui::DockBuilderSplitNode (dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
      ImGuiID dock_down_right_id =
          ImGui::DockBuilderSplitNode (dock_down_id, ImGuiDir_Right, 0.5f, nullptr, &dock_down_id);

      ImGui::DockBuilderDockWindow ("Inspector", dock_right_id);
      ImGui::DockBuilderDockWindow ("TilesetEditor", dock_right_down_id);
      ImGui::DockBuilderDockWindow ("Chat", dock_down_id);
      ImGui::DockBuilderDockWindow ("Console", dock_down_right_id);
      ImGui::DockBuilderDockWindow ("Scene", dock_main_id);

      ImGui::DockBuilderFinish (dock_main_id);
    }

    ImGui::DockSpace (dockspace_id, ImVec2 (0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
    this->draw_menu_bar();
    ImGui::End();

    m_scene.render ((void*) (intptr_t) m_FBO->GetTexture());
    
    if (m_toolbar.is_open())
    {
      m_toolbar.render (m_current_state, m_current_tool);
    }
    if (m_inspector.is_open())
    {
      m_inspector.render (m_game.m_registry);
    }
    if (m_map_editor.is_open())
    {
      m_map_editor.render();
    }
    if (m_tileset_editor.is_open())
    {
      m_tileset_editor.render();
    }
    if (m_chat.is_open())
    {
      m_chat.render (m_registry);
    }
    if (m_console.is_open())
    {
      m_console.render ();
    }
    if (m_view_physics_debug_layer)
    {
      // this->draw_info (info_pos);
      // m_debug_layer.Render();
    }
  }

  void EditorGui::draw_info (const ImVec2& pos)
  {
    ImGui::SetNextWindowBgAlpha (0.4);
    ImGui::SetNextWindowPos (pos, ImGuiCond_Always);
    ImGui::Begin ("Info", nullptr, m_window_flags | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::PushFont (m_font_sans_bold);
    ImGui::Text ("  - Stella Engine -  ");
    ImGui::PopFont();
    ImGui::Dummy (ImVec2 (0.0f, 3.0f));
    ImGui::Separator();
    ImGui::Dummy (ImVec2 (0.0f, 6.0f));
    ImGui::Text ("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::Text ("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Dummy (ImVec2 (0.0f, 3.0f));
    ImGui::End();
  }

  void EditorGui::draw_menu_bar()
  {
    if (ImGui::BeginMenuBar())
    {
      if (ImGui::BeginMenu ("File"))
      {
        if (ImGui::MenuItem ("New Tilemap", "CTRL+N")) {}
        if (ImGui::MenuItem ("Open Tilemap", "CTRL+O")) {}
        if (ImGui::MenuItem ("Open Tileset", "SHIFT+O")) {}
        ImGui::Separator();
        if (ImGui::MenuItem ("Save", "CTRL+S")) {}
        if (ImGui::MenuItem ("Save as...", "CTRL+SHIFT+S")) {}
        ImGui::Separator();
        if (ImGui::MenuItem ("Quit", "CTRL+W"))
        {
          m_game.m_display.Running = false;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("Edit"))
      {
        if (ImGui::MenuItem ("Undo", "CTRL+Z")) {}
        ImGui::Separator();
        if (ImGui::MenuItem ("Redo", "CTRL+Y", false, false)) {} // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem ("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("View"))
      {
        auto item_text = "View Physics debug layer";
        if (m_view_physics_debug_layer)
        {
          item_text = "Hide Physics debug layer";
        }
        if (ImGui::MenuItem (item_text, "CTRL+D"))
        {
          m_view_physics_debug_layer = !m_view_physics_debug_layer;
        }

        item_text = "View Inspector";
        if (m_inspector.is_open())
        {
          item_text = "Hide Inspector";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_inspector.toggle();
        }

        item_text = "View Map Editor";
        if (m_map_editor.is_open())
        {
          item_text = "Hide Map Editor";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_map_editor.toggle();
        }

        item_text = "View Tileset Editor";
        if (m_tileset_editor.is_open())
        {
          item_text = "Hide Tileset Editor";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_tileset_editor.toggle();
        }

        item_text = "View Toolbar";
        if (m_toolbar.is_open())
        {
          item_text = "Hide Toolbar";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_toolbar.toggle();
        }

        item_text = "View Chat";
        if (m_chat.is_open())
        {
          item_text = "Hide Chat";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_chat.toggle();
        }

        item_text = "View Console";
        if (m_console.is_open())
        {
          item_text = "Hide Console";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_console.toggle();
        }

        item_text = "Return to Editor";
        if (m_show_editor)
        {
          item_text = "Play Game";
        }
        if (ImGui::MenuItem (item_text))
        {
          m_show_editor = !m_show_editor;
        }
        ImGui::EndMenu();
      }
      if (ImGui::BeginMenu ("Tools"))
      {
        if (ImGui::MenuItem ("Redo", "CTRL+Y", false, false)) {} // Disabled item
        ImGui::Separator();
        if (ImGui::MenuItem ("Paste", "CTRL+V")) {}
        ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
    }
  }
} // namespace editor
} // namespace stella
