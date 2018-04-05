#pragma once

#include <stella/physics2d/body.h>

struct Body2DComponent {
  Body2DComponent() {}
  ~Body2DComponent() {}

  std::shared_ptr<stella::physics2d::Body> Body;
  bool Initialized = false;
};

