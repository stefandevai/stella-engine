#pragma once

#include "../texture.hpp"
#include "./shape_layer.hpp"

namespace stella
{
namespace graphics
{
  class WaterLayer : public graphics::ShapeLayer
  {
  public:
    WaterLayer (float width, float height, bool fixed = true);
    ~WaterLayer();
    void Render() override;
  };
} // namespace graphics
} // namespace stella
