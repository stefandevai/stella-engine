#pragma once

#include <anax/System.hpp>
#include <stella/graphics/renderer.h>
#include "scenelayer.h"

#include "position_component.h"
#include "sprite_component.h"

class RenderingSystem : public anax::System<anax::Requires<PositionComponent, SpriteComponent>>
{
  public:
    RenderingSystem(int width, int height, stella::graphics::Shader *shad);
    ~RenderingSystem();
    void Prepare();
    void Render();

  private:
    SceneLayer* TileLayer;
    stella::graphics::Shader *shader;
};

