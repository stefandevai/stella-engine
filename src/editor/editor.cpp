#include "editor/editor.hpp"
#include "imgui_internal.h"

#include "../../nikte/game.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/layer.hpp"
#include "stella/components/position.hpp"
#include "stella/components/sprite.hpp"

#ifdef _WIN32
  #include <SDL.h>
#else
  #include <SDL2/SDL.h> // IWYU pragma: export
#endif
#undef main
#include <cereal/cereal.hpp> // IWYU pragma: export

namespace stella
{
namespace editor
{
  Editor::Editor (nikte::Game& game) : m_game (game), m_registry (game.m_registry)
  {
    
    m_debug_layer = std::make_shared<graphics::ShapeLayerT>(m_registry, "assets/shaders/debug_shader.vert", "assets/shaders/debug_shader.frag", true);
    // m_game.m_display.SetEditor (this);
    // m_debug_layer.Add(shape);
    m_editor_layer = game.m_registry.create();
    game.m_registry.emplace<component::LayerT> (m_editor_layer, "editor", 9999, component::LayerType::SPRITE_LAYER, "assets/shaders/sprite_batch.vert", "assets/shaders/sprite_batch.frag", false);

    ImVec2 dimensions = m_tileset_editor.get_tile_dimensions();
    m_editor_sprite   = game.m_registry.create();
    game.m_registry.emplace<component::Position> (m_editor_sprite, -dimensions.x, -dimensions.y);
    game.m_registry.emplace<component::Dimension> (m_editor_sprite, dimensions.x, dimensions.y);
    auto& sprite = game.m_registry.emplace<component::SpriteT> (m_editor_sprite, "editor");

    sprite.hframes = m_tileset_editor.get_texture_dimensions_in_tiles().x;
    sprite.vframes = m_tileset_editor.get_texture_dimensions_in_tiles().y;
    sprite.texture_ptr = std::make_shared<graphics::Texture>(m_tileset_editor.texture);
    sprite.frame = 0;
    sprite.layer = "editor";
    this->init();
  }

  Editor::~Editor()
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }

  void Editor::init()
  {
    m_window = m_game.m_display.Window;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io    = ImGui::GetIO();
    io.IniFilename = "config/imgui.ini";

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

  void Editor::configure_input()
  {
    ImGui_ImplSDL2_ProcessEvent (&m_game.m_display.m_event);
    const Uint8* state = SDL_GetKeyboardState (nullptr);

    switch (m_current_state)
    {
      case EDIT:
      {
        // Save map
        if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_S])
        {
          m_map_editor.save_as();
        }
        else if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_S])
        {
          m_map_editor.save();
        }

        // Load map
        if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_O])
        {
          m_map_editor.load();
        }

        // Run game without the editor
        if (!state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_R])
        {
          m_play_mode();
        }

        // Pan tool
        if (state[SDL_SCANCODE_SPACE] && ImGui::IsMouseDragging (0))
        {
          ImGuiIO& io = ImGui::GetIO();
          m_handle_pan_tool (io);
        }
      }
      break;

      case PLAY:
      {
        // Get back to edit mode
        if (state[SDL_SCANCODE_LCTRL] && state[SDL_SCANCODE_LSHIFT] && state[SDL_SCANCODE_R])
        {
          m_edit_mode();
        }
      }
      break;
      default:
        break;
    }
  }

  void Editor::run()
  {
    while (m_game.m_display.IsRunning())
    {
      if (m_current_state == EDIT)
      {
        m_FBO->Bind();
        m_game.m_display.Clear();
        m_game.update (m_game.m_display.GetDT());
        m_FBO->Unbind();
        this->render (m_game.m_display.GetWindowWidth(),
                      m_game.m_display.GetWindowHeight(),
                      m_game.m_display.Width,
                      m_game.m_display.Height);

        m_game.m_display.Update();
        this->configure_input();
      }
      else if (m_current_state == PLAY)
      {
        m_game.m_display.Clear();
        m_game.update (m_game.m_display.GetDT());
        this->render (m_game.m_display.GetWindowWidth(),
                      m_game.m_display.GetWindowHeight(),
                      m_game.m_display.Width,
                      m_game.m_display.Height);
        m_game.m_display.Update();
        this->configure_input();
      }
    }
  }

  void Editor::update() { m_log_system.update (m_registry, 0.0); }

  void
  Editor::render (const float window_width, const float window_height, const float game_width, const float game_height)
  {
    if (m_window != nullptr)
    {
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplSDL2_NewFrame (m_window);
      ImGui::NewFrame();
      ImGui::PushFont (m_font_sans_regular);

      if (m_current_state == EDIT)
      {
        this->draw_dock (window_width, window_height, game_width, game_height);
      }
      m_toolbar.render (m_current_state, m_current_tool, m_window_width, [this]() { this->draw_menu_bar(); });

      ImGui::PopFont();
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData (ImGui::GetDrawData());
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
            this->m_handle_tile_pen (io);
            break;
          case INSPECTOR:
            this->m_handle_inspector (io);
            break;
          case PAN:
            this->m_handle_pan_tool (io);
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

  void Editor::m_map_tile_pos (const ImGuiIO& io, std::function<void (const ImVec2&)> position_action)
  {
    // If the mouse is within the game screen boundaries
    if (m_scene.active() && io.MousePos.x > m_scene.get_x() && io.MousePos.x < m_scene.get_width() &&
        io.MousePos.y <= m_scene.get_game_screen_height() + m_scene.get_game_screen_y_spacing() &&
        io.MousePos.y > m_scene.get_game_screen_y_spacing())
    {
      const auto& camera_pos    = m_game.get_camera_pos();
      float width_padding       = m_scene.get_game_screen_x_spacing();
      float height_padding      = m_scene.get_game_screen_y_spacing();
      const float width_factor  = m_game_width / static_cast<float> (m_scene.get_game_screen_width());
      const float height_factor = m_game_height / static_cast<float> (m_scene.get_game_screen_height());

      const auto map_pos_x = (io.MousePos.x - width_padding) * width_factor + camera_pos[0] - 1.0f;
      const auto map_pos_y = (io.MousePos.y - height_padding) * height_factor + camera_pos[1] - 3.0f;

      position_action (ImVec2 (map_pos_x, map_pos_y));
    }
  }

  void Editor::m_handle_tile_pen (const ImGuiIO& io)
  {
    m_map_tile_pos (io, [this] (const ImVec2& map_pos) {
      // Set dummy sprite position with grid snapping
      int new_tile_value  = m_tileset_editor.get_selected_tile_id();
      const auto tile_pos = m_tileset_editor.pos2tile (map_pos.x, map_pos.y);
      auto& sprite_pos    = m_registry.get<component::Position> (m_editor_sprite);
      auto& sprite_spr    = m_registry.get<component::SpriteT> (m_editor_sprite);
      sprite_pos.x        = tile_pos.x * m_tileset_editor.get_tile_dimensions().x;
      sprite_pos.y        = tile_pos.y * m_tileset_editor.get_tile_dimensions().y;
      sprite_spr.frame = new_tile_value;

      if (ImGui::IsMouseDown (0))
      {
        // Update tile if user clicks
        bool collidable = m_tileset_editor.get_selected_tile_collidable();
        int layer_id    = m_map_editor.get_selected_layer_id();
        m_game.m_tile_map.update_tile (new_tile_value, tile_pos.x, tile_pos.y, layer_id, collidable);
      }
    });
  }

  void Editor::m_handle_pan_tool (const ImGuiIO& io)
  {
    m_map_tile_pos (io, [this] (const ImVec2& map_pos) {
      auto& pos = m_registry.get<stella::component::Position> (m_game.m_camera);
      if (!is_panning)
      {
        camera_pos_without_pan = ImVec2{pos.x, pos.y};
        is_panning             = true;
      }
      ImVec2 drag = ImGui::GetMouseDragDelta();
      pos.x       = camera_pos_without_pan.x - drag.x;
      pos.y       = camera_pos_without_pan.y - drag.y;
    });
  }

  void Editor::m_handle_inspector (const ImGuiIO& io)
  {
    m_map_tile_pos (io, [this] (const ImVec2& map_pos) {
      if (ImGui::IsMouseClicked (0))
      {
        // Sort by z value before getting the right entity
        m_game.m_registry.sort<component::Position> ([] (const auto& lhs, const auto& rhs) { return lhs.z < rhs.z; });
        // TODO: Find a better way to select entity based on position
        m_game.m_registry.view<stella::component::Position, stella::component::Dimension, stella::component::SpriteT>()
            .each ([this, &map_pos] (auto entity, auto& pos, auto& dim, auto& spr) {
              if (m_game.m_registry.valid (entity) && map_pos.x >= pos.x && map_pos.x < pos.x + dim.w &&
                  map_pos.y >= pos.y && map_pos.y < pos.y + dim.h)
              {
                m_inspector.set_selected_entity (entity);
                return;
              }
            });
      }
    });
  }

  void Editor::m_play_mode()
  {
    m_current_state = PLAY;
    glViewport (0,
                m_toolbar.size().y,
                m_game.m_display.GetWindowWidth(),
                m_game.m_display.GetWindowHeight() - m_toolbar.size().y);
    m_game.m_display.m_check_viewport_proportions();
  }

  void Editor::m_edit_mode()
  {
    m_current_state = EDIT;
    glViewport (0, 0, m_game_width, m_game_height);
  }

  void Editor::init_style()
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

  void Editor::draw_dock (const float window_width,
                          const float window_height,
                          const float game_width,
                          const float game_height)
  {
    m_window_width  = window_width;
    m_window_height = window_height;
    m_game_width    = game_width;
    m_game_height   = game_height;

    ImGuiIO& io = ImGui::GetIO();
    m_handle_state (io);

    float dock_width  = window_width;
    float dock_height = window_height;
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
    ImGui::End();

    m_scene.render ((void*) (intptr_t) m_FBO->GetTexture());

    m_map_editor.render_file_dialog();

    if (m_inspector.is_open())
    {
      m_inspector.render (m_game.m_registry);
    }
    m_map_editor.render();
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
      m_console.render();
    }
    if (m_view_physics_debug_layer)
    {
      // this->draw_info (info_pos);
      // m_debug_layer.Render();
    }
  }

  void Editor::draw_info (const ImVec2& pos)
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

  void Editor::draw_menu_bar()
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
        if (ImGui::MenuItem ("Quit", "CTRL+Q"))
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
        auto item_text = "Physics debug layer";
        if (m_view_physics_debug_layer)
        {
          item_text = "Hide Physics debug layer";
        }
        if (ImGui::MenuItem (item_text, "CTRL+D"))
        {
          m_view_physics_debug_layer = !m_view_physics_debug_layer;
        }
        ImGui::Dummy (ImVec2{0.0f, 1.5f});
        ImGui::Separator();
        ImGui::Dummy (ImVec2{0.0f, 1.5f});
        m_widget_build_option (m_inspector);
        ImGui::Dummy (ImVec2{0.0f, 3.0f});
        m_widget_build_option (m_map_editor);
        ImGui::Dummy (ImVec2{0.0f, 3.0f});
        m_widget_build_option (m_tileset_editor);
        ImGui::Dummy (ImVec2{0.0f, 3.0f});
        m_widget_build_option (m_chat);
        ImGui::Dummy (ImVec2{0.0f, 3.0f});
        m_widget_build_option (m_console);
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

  void Editor::m_widget_build_option (widget::Widget& widget)
  {
    auto item_text = widget.get_name();
    if (widget.is_open())
    {
      item_text = "Hide " + widget.get_name();
    }
    if (ImGui::MenuItem (item_text.c_str()))
    {
      widget.toggle();
    }
  }
} // namespace editor
} // namespace stella
