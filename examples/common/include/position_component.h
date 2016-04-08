#pragma once

#include <anax/Component.hpp>

struct PositionComponent : public anax::Component
{
  public:
    inline PositionComponent(int px, int py) : x(px), y(py) {  }
    int x, y;
};

