#pragma once

struct SpatialComponent {
  SpatialComponent(int w, int h, int x = 0, int y = 0)
      : w(w), h(h), x(x), y(y) {
    tx = (int)x / w;
    ty = (int)y / h;
  }
  int w, h, x, y, ty, tx;
};
