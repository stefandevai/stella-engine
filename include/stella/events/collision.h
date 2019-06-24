#pragma once

#include <entityx/entityx.h>
namespace ex = entityx;

struct Collision {
  Collision(ex::Entity fire, ex::Entity snowflake)
      : Fire(fire), Snowflake(snowflake) {}
  ex::Entity Fire, Snowflake;
};

