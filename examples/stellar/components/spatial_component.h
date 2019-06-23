#pragma once

struct SpatialComponent {
  SpatialComponent(int w, int h, int x = 0, int y = 0)
      : w(w), h(h), x(x), y(y) {}
  int w, h, x, y;
};
