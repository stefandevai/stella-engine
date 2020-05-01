#pragma once

#include <vector>

namespace stella
{
namespace graphics
{
  class Sprite;

  class Renderer
  {
  public:
    Renderer() {}
    
    virtual void Begin() = 0;
    virtual void Submit (const Sprite& sprite) = 0;
    virtual void End() = 0;
    virtual void Draw() = 0;
  protected:
    virtual ~Renderer() {}
  };
} // namespace graphics
} // namespace stella
