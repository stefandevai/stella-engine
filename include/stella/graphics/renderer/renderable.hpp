#pragma once

namespace stella::graphics
{
  class ShaderProgram;

  class Renderable
  {
  public:
    virtual ~Renderable() {}

    virtual void render (ShaderProgram& shader) = 0;
  };
}
