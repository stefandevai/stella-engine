#pragma once

#include "./shape_layer.h"
#include "../texture.h"

namespace stella
{
namespace graphics
{
  class WaterLayer : public graphics::ShapeLayer
  {
    public:
      WaterLayer(float width, float height, bool fixed = true);
      ~WaterLayer();
      void Render() override;
  };
}
}


