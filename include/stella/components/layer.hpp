#pragma once

#include "component.hpp"

#include <string>

namespace stella
{
namespace component
{
  enum LayerType
  {
      SPRITE_LAYER,
      SHAPE_LAYER
  };

  struct LayerT : public Component
  {
    LayerT() : Component ("Layer") {}
    LayerT (std::string id,
           unsigned order,
           LayerType layer_type,
           std::string vert_shader_source,
           std::string frag_shader_source,
           bool fixed = false)
      : Component ("Layer"), id (id), order (order), layer_type (layer_type), vert_shader_source (vert_shader_source),
        frag_shader_source (frag_shader_source), fixed (fixed)
    {
    }

    std::string id = "";
    unsigned order = 0;
    LayerType layer_type = SPRITE_LAYER;
    std::string vert_shader_source = "assets/shaders/sprite_batch.vert", frag_shader_source = "assets/shaders/sprite_batch.frag";
    bool fixed = false;
    bool initialized = false;
  };

} // namespace component
} // namespace stella
