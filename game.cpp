#include <iostream>
#include "src/stella.h"

int main(int argc, const char *argv[])
{
  stella::graphics::Display display(640, 480, "Stella");
  while (display.IsRunning())
  {
    display.Update();
  }
  return 0;
}

