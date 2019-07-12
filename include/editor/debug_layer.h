#pragma once

#include "stella/graphics/layers/layer.h"
#include "stella/graphics/texture.h"

namespace stella
{
namespace editor
{
  class DebugLayer : public graphics::Layer
  {
    public:
      DebugLayer(float width, float height, bool fixed = true);
      ~DebugLayer();
      void Render() override;
  };
}
}

