#include <stella/graphics/display.h>

#include "game.h"

int main(int argc, char *argv[]) {

  stella::graphics::Display display(896, 504, "T O R C H");
  display.SetClearColor(0, 0, 0);

  Game game(display, argc, argv);

  while (display.IsRunning()) {
    display.Clear();
    game.Update(display.GetDT());
    display.Update();
  }

  return 0;
}

