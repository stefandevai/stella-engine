#pragma once

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

  struct LayerT
  {
    LayerT (std::string id,
           unsigned order,
           LayerType layer_type,
           std::string vert_shader_source,
           std::string frag_shader_source,
           bool fixed = false)
      : id (id), order (order), layer_type (layer_type), vert_shader_source (vert_shader_source),
        frag_shader_source (frag_shader_source), fixed (fixed)
    {
    }

    std::string id;
    unsigned order;
    LayerType layer_type;
    std::string vert_shader_source, frag_shader_source;
    bool fixed;
    bool initialized = false;
  };

} // namespace component
} // namespace stella
