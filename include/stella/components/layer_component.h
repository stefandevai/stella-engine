#pragma once

namespace stella
{
namespace components
{
  struct LayerComponent
  {
    LayerComponent (std::string id,
                    unsigned order,
                    std::string shader_id,
                    std::string vert_shader_source,
                    std::string frag_shader_source,
                    bool fixed = false)
      : Id (id), Order (order), ShaderId (shader_id), vert_shader_source (vert_shader_source),
        frag_shader_source (frag_shader_source), Fixed (fixed)
    {
    }

    std::string Id;
    unsigned Order;
    std::string ShaderId;
    std::string vert_shader_source, frag_shader_source;
    bool Fixed;
    bool Initialized = false;
  };

} // namespace components
} // namespace stella
