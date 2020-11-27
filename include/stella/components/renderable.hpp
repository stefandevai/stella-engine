#pragma once

#include "component.hpp"
#include "stella/graphics/renderer/renderer.hpp"

namespace stella::component
{

  struct Renderable : public Component
  {
    Renderable (graphics::RendererType renderer_type) : Component ("Renderable"), renderer_type (renderer_type) {}

    graphics::RendererType renderer_type = graphics::RendererType::NONE;
  };

} // namespace stella

