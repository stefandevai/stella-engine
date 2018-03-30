#include <iostream>
#include <array>
#include <stella/stella.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "game.h"

void DrawQuad(GLuint &VAO, GLuint &VBO);

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    std::cout << "It was not possible to initialize SDL2" << std::endl;

  SDL_Window* window = SDL_CreateWindow("S T E L L A R", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_CreateContext(window);

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  const GLubyte* renderer = glGetString(GL_RENDERER);
  const GLubyte* version = glGetString(GL_VERSION);
  std::cout << "Renderer: " << renderer << std::endl;
  std::cout << "OpenGL version supported: " << version << std::endl;

  bool running = true;
  while(running) {
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        running = false;
      if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          running = false;
          break;
        default:
          break;
        }
      }
    }
   SDL_GL_SwapWindow(window);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();


  //stella::graphics::Display display(720, 405, "S T E L L A R");
  //display.SetClearColor(0, 0, 0);

  //Game game(display);

  //while (display.IsRunning()) {
    //display.Clear();
    //game.Update(display.GetDT());
    //display.Update();
  //}

  return 0;
}

