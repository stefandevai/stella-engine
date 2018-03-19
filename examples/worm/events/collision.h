#include <entityx/entityx.h>

struct Collision {
  Collision(entityx::Entity dynamic_entity, entityx::Entity static_entity)
      : Dynamic(dynamic_entity), Static(static_entity) {}
  entityx::Entity Dynamic, Static;
};
