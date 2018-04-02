#include "collision_system.h"

#include <algorithm>
#include "../events/collision.h"

CollisionSystem::CollisionSystem(int w, int h) {
}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  std::vector<entityx::Entity> fire_particles;
  std::vector<entityx::Entity> snow_particles;

  fire_particles.clear();
  snow_particles.clear();

  es.each<ParticleComponent>(
      [this, &fire_particles, &snow_particles](
          entityx::Entity entity, ParticleComponent &par) {
          if (par.ID == -1)
            fire_particles.emplace_back(entity);
          else if (par.ID == 1)
            snow_particles.emplace_back(entity);
      });

  for (auto b1: fire_particles) {
    for (auto b2: snow_particles) {
      if (b1.valid() && b2.valid()) {
        if (check_collision(b1, b2)) {
          events.emit<Collision>(b1, b2);
        }
      }
    }
  }

  //for (auto b1 = fire_particles.begin(); b1 != fire_particles.end(); ++b1) {
    //for (auto b2 = snow_particles.begin(); b2 != snow_particles.end(); ++b2) {
      //if ((*b1).valid() && (*b2).valid()) {
        //if (check_collision(*b2, *b1)) {
          //(*b2).destroy();
          //snow_particles.erase(b2);
          ////std::cout << "here\n";
          ////events.emit<Collision>(*b1, *b2);
        //}
      //}
    //}
  //}
}

bool CollisionSystem::check_collision(entityx::Entity &b1,
                                      entityx::Entity &b2) {
  auto pos1 = b1.component<PositionComponent>();
  auto dim1 = b1.component<DimensionComponent>();
  auto pos2 = b2.component<PositionComponent>();
  auto dim2 = b2.component<DimensionComponent>();

  bool colX = pos1->x + dim1->w > pos2->x && pos2->x + dim2->w > pos1->x;
  bool colY = pos1->y + dim1->h > pos2->y && pos2->y + dim2->h > pos1->y;

  return colX && colY;
}

