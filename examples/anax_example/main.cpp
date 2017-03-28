#include <anax/anax.hpp>
#include <stella/stella.h>
#include <iostream>

#include "position_component.h"
#include "sprite_component.h"
#include "rendering_system.h"

int main(int argc, char *argv[])
{
  GLboolean Keys[1024] = { 0 };
  stella::graphics::Display display(800, 600, "Stella", Keys);
  display.SetClearColor(22, 38, 47);

  // TODO Move this block to init on singleton class
  GLint tex_ids[] =
  {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
  };

  stella::graphics::Shader shader("assets/shaders/basic_shader.vsh", "assets/shaders/basic_shader.fsh");
  shader.Enable();
  shader.SetIntv("textures", tex_ids, 10);
  shader.Disable();
  // End of block

  anax::World world;
  anax::Entity entity = world.createEntity();
  entity.addComponent<PositionComponent>((int)display.GetWidth()/2 - 23, (int)display.GetHeight()/2 - 51);
  stella::graphics::Texture StellaTex("stella-tex", "assets/gfx/sprites/tina.png");
  entity.addComponent<SpriteComponent>(46, 102, StellaTex, 0);
  entity.activate();

  RenderingSystem renderingSystem(display.GetWidth(), display.GetHeight(), &shader);
  world.addSystem(renderingSystem);
  world.refresh();
  renderingSystem.Prepare();

  while(display.IsRunning())
  {
    display.Clear();

    world.refresh();
    renderingSystem.Render();

    display.Update();
  }

  entity.kill();

  return 0;
}

