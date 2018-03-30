#include <iostream>
#include <array>
#include <stella/stella.h>
#include <stella/particles.h>

#include "game.h"

void DrawQuad(GLuint &VAO, GLuint &VBO);

int main(int argc, char *argv[]) {
  stella::graphics::Display display(720, 405, "S T E L L A R");
  display.SetClearColor(0, 0, 0);

  Game game(display);

  while (display.IsRunning()) {
    display.Clear();
    game.Update(display.GetDT());
    display.Update();
  }

  return 0;
}

