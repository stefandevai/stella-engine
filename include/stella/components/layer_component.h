#pragma once

namespace stella {
namespace components {

struct LayerComponent {
  LayerComponent(std::string id, unsigned order, std::string shader_id = "basic", bool fixed = false)
    : Id(id), Order(order), ShaderId(shader_id), Fixed(fixed)
  {

  }

  std::string Id;
  unsigned Order;
  std::string ShaderId;
  bool Fixed;
  bool Initialized = false;
};

} // namespace components
} // namespace stella


