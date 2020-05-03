#pragma once

#include <entt/entity/entity.hpp>
#include <entt/entity/registry.hpp>
namespace stella{ namespace graphics{ class Texture; } }

namespace stella
{
namespace graphics
{
  using TexPtrs = std::vector<std::shared_ptr<Texture>>;

  class RendererT
  {
  public:
    RendererT() {}
    
    virtual void begin() = 0;
    virtual void submit (entt::registry& registry, entt::entity entity) = 0;
    virtual void end() = 0;
    virtual void draw() = 0;

  protected:
    virtual ~RendererT() {}
    static TexPtrs m_textures;
  };
} // namespace graphics
} // namespace stella
