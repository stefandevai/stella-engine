#include <iostream>
#include "glm/glm/glm.hpp"
#include "src/stella.h"

int main(int argc, const char *argv[])
{
  using namespace stella;
  using namespace graphics;

  std::string file1 = read_file("assets/shaders/basic_shader.vs");
  std::cout << file1 << std::endl;

  Display display(640, 480, "Stella");
  display.SetClearColor(204, 102, 51);

  while (display.IsRunning())
  {
    display.Update();
  }
  return 0;
}

