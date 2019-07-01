#pragma once

namespace stella {
namespace components {
struct LayerComponent {
  LayerComponent(std::string id, unsigned order, std::string shader_id = "basic") : Id(id), Order(order), ShaderId(shader_id) {}
  std::string Id;
  unsigned Order;
  std::string ShaderId;
  bool Initialized = false;
  bool Fixed = false;
};
} // namespace components
} // namespace stella


