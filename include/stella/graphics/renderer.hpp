#pragma once

#include <vector>
#include <memory>

namespace stella
{
namespace graphics
{
  class Renderable;
  class Texture;

  class Renderer
  {
  public:
    Renderer() {}
    
    virtual void Begin() = 0;
    virtual void Submit (const std::shared_ptr<Renderable> renderable) = 0;
    virtual void End() = 0;
    virtual void Draw() = 0;
    static std::vector<Texture*> Textures;

  protected:
    virtual ~Renderer() {}
  };
} // namespace graphics
} // namespace stella
