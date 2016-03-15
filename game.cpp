#include <iostream>
#include "glm/glm/glm.hpp"
#include "src/stella.h"

int main(int argc, const char *argv[])
{
  stella::graphics::Display display(640, 480, "Stella");
  display.SetClearColor(204, 102, 51);

  while (display.IsRunning())
  {
    display.Update();
  }
  return 0;
}

