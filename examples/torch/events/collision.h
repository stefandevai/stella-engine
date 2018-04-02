#pragma once

#include <entityx/entityx.h>

struct Collision {
  Collision(entityx::Entity fire, entityx::Entity snowflake)
      : Fire(fire), Snowflake(snowflake) {}
  entityx::Entity Fire, Snowflake;
};

