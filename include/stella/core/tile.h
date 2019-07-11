#pragma once

namespace stella
{
namespace core
{

  struct Tile
  {
    Tile(const int value = 0, const bool visible = false) : value(value), visible(visible) {}
    int value = 0;
    bool visible = false;
  };

}
}


