#pragma once

namespace stella {
namespace components {
struct TileComponent {
  TileComponent(unsigned layer_id) : layer_id(layer_id) {}
  unsigned layer_id = 0;
};
} // namespace components
} // namespace stella

