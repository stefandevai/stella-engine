#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <entt/entity/registry.hpp>
#include "./log_system.h"
#include "./tileset_editor.h"
#include "./gui_log.h"
#include "./console.h"
#include "./debug_layer.h"
#include "../stella/graphics/shape.h"
//#include "../stella/core/game.h"

struct SDL_Window;
union SDL_Event;
typedef void *SDL_GLContext;
namespace nikte
{
  class Game;
}

namespace stella
{
  
namespace editor
{

  class EditorGui {
    private:
      enum State { EDIT, PLAY };
      enum Tool { TILE_PEN };

      State m_current_state = EDIT;
      Tool m_current_tool = TILE_PEN;

      nikte::Game &m_game;
      SDL_Window *m_window = nullptr;
      ImFont *m_font_mono = nullptr, *m_font_sans_regular = nullptr, *m_font_sans_bold = nullptr;
      const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;
      GuiLog m_log{m_window_flags, m_font_mono};
      Console m_console{m_window_flags, m_font_mono, m_registry};
      LogSystem m_log_system{m_log};
      entt::registry &m_registry;
      DebugLayer m_debug_layer{896, 504, true};
      TilesetEditor m_tileset_editor{"assets/sprites/tileset.png"};
      float m_window_width = 0.f, m_window_height = 0.f, m_game_width = 0.f, m_game_height = 0.f;
 
      //graphics::Texture texture_placeholder{"assets/sprites/player.png"};
      std::vector<glm::vec2> vertices{glm::vec2{0.0f, 20.0f}, glm::vec2{128.0f, 80.0f}, glm::vec2{128.0f, 128.0f}, glm::vec2{0.0f, 128.0f}};
      std::shared_ptr<graphics::Shape> shape = std::make_shared<graphics::Shape>(vertices, glm::vec3{200.f, 100.f, 1.f});
      //std::shared_ptr<graphics::Sprite> sprite_placeholder = std::make_shared<graphics::Sprite>(300, 300, 100, 100, texture_placeholder);

      // View options
      bool m_view_physics_debug_layer = false;
      bool m_tile_editor = false;


    public:
      //EditorGui(entt::registry& registry);
      EditorGui(nikte::Game &game);
      ~EditorGui();
      void init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version);
      void configure_input(SDL_Event &event);
      void render(const float window_width, const float window_height, const float game_width, const float game_height);
      void update();
      void clean();

    private:
      void init_style();
      void draw_editor(const ImVec2 &size, const ImVec2 &pos);
      void draw_console(const ImVec2 &size, const ImVec2 &pos);
      void draw_log();
      void draw_info(const ImVec2 &pos);
      void draw_menu_bar();
      void handle_state(ImGuiIO& io);
  };

}
}

