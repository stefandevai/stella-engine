#pragma once

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>

struct SDL_Window;
union SDL_Event;
typedef void *SDL_GLContext;

namespace stella
{
namespace editor
{

  class EditorGui {
    private:
      SDL_Window *m_window = nullptr;
      ImFont *m_font_mono, *m_font_sans_regular, *m_font_sans_bold;
      const ImGuiWindowFlags m_window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

    public:
      EditorGui();
      ~EditorGui();
      void init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version);
      void configure_input(SDL_Event &event);
      void render(const float window_width, const float window_height, const float game_width, const float game_height);
      void clean();

    private:
      void init_style();
      void draw_editor(const ImVec2 &size, const ImVec2 &pos);
      void draw_console(const ImVec2 &size, const ImVec2 &pos);
      void draw_info(const ImVec2 &pos);
      void draw_menu_bar();
  };

}
}

