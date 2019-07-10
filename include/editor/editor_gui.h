#pragma once

struct SDL_Window;
union SDL_Event;
typedef void *SDL_GLContext;

namespace stella
{
namespace editor
{

  class EditorGui {
    public:
      EditorGui();
      ~EditorGui();
      void init(SDL_Window *window, SDL_GLContext gl_context, const char *glsl_version);
      void configure_input(SDL_Event &event);
      void render();
      void clean();
    private:
      SDL_Window *m_window = nullptr;
  };

}
}

