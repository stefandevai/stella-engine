#include <iostream>
#include <array>
#include <stella/stella.h>

#include "game.h"

int main(int argc, char *argv[]) {
  stella::graphics::Display display(720, 405, "W O R M !");
  display.SetClearColor(16 / 255.0f, 5 / 255.0f, 21 / 255.0f);

  Game game(display);

  while (display.IsRunning()) {
    display.Clear();
    game.Update(display.GetDT());
    display.Update();
  }

  return 0;
}
