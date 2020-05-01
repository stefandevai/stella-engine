#pragma once

#include "stella/graphics/layers/shape_layer.hpp"
#include "stella/graphics/texture.hpp"

namespace stella
{
namespace editor
{
  class DebugLayer : public graphics::ShapeLayer
  {
  public:
    DebugLayer (float width, float height, bool fixed = true);
    ~DebugLayer();
    void Render() override;
  };
} // namespace editor
} // namespace stella