#include "collision_system.h"

#include "../components/sprite_component.h"
#include "../events/collision.h"

CollisionSystem::CollisionSystem(int w, int h) {}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  std::vector<entityx::Entity> static_bodies;
  std::vector<entityx::Entity> dynamic_bodies;

  static_bodies.clear();
  dynamic_bodies.clear();

  es.each<SpatialComponent, BodyComponent>(
      [this, &static_bodies, &dynamic_bodies](
          entityx::Entity entity, SpatialComponent &spa, BodyComponent &body) {
        if (body.IsStatic)
          static_bodies.push_back(entity);
        else
          dynamic_bodies.push_back(entity);
      });

  for (auto b1 : dynamic_bodies) {
    for (auto b2 : static_bodies) {
      if (b1.valid() && b2.valid()) {
        if (check_collision(b1, b2)) {
          events.emit<Collision>(b1, b2);
        }
      }
    }
  }
}

bool CollisionSystem::check_collision(entityx::Entity &b1,
                                      entityx::Entity &b2) {
  auto spa1 = b1.component<SpatialComponent>();
  auto spa2 = b2.component<SpatialComponent>();

  bool colX = spa1->x + spa1->w > spa2->x && spa2->x + spa2->w > spa1->x;
  bool colY = spa1->y + spa1->h > spa2->y && spa2->y + spa2->h > spa1->y;

  return colX && colY;
}
