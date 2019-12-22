#pragma once

#include <bitset>

namespace stella
{
namespace core
{

  struct Tile
  {
    Tile(const int value = 0) : value(value) {}
    int x = 0;
    int y = 0;
    int value = 0;
    bool visible = false;
    std::bitset<4> solid_edges;
    std::bitset<4> custom_edges;
    std::bitset<4> active_edges;
  };

}
}


