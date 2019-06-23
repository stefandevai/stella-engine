#include "collision_system.h"

#include <algorithm>

CollisionSystem::CollisionSystem(int w, int h) {
  Width = w / PARTITIONS + 1;
  Height = h / PARTITIONS + 1;
}

CollisionSystem::~CollisionSystem() {}

void CollisionSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  // Clear collision grid each frame
  grid.clear();
  grid.resize(Width * Height);

  es.each<SpatialComponent, BodyComponent>([this](entityx::Entity entity,
                                                  SpatialComponent &spa,
                                                  BodyComponent &body) {
    // Build collision grid
    makeCollisionGrid(entity, spa, body);
  });

  // Check for collosions on all candidates
  for (auto candidates = grid.begin(); candidates != grid.end(); ++candidates) {
    for (auto c1 = (*candidates).begin(); c1 != (*candidates).end(); ++c1) {
      for (auto c2 = c1; c2 != (*candidates).end(); ++c2) {
        if ((*c1).entity != (*c2).entity && collided(*c1, *c2)) {
          resolveCollision((*c1).entity, (*c2).entity);
        }
      }
    }
  }
}

void CollisionSystem::makeCollisionGrid(entityx::Entity &entity,
                                        SpatialComponent &spa,
                                        BodyComponent &body) {
  unsigned int left = spa.x / PARTITIONS, right = (spa.x + spa.w) / PARTITIONS,
               top = spa.y / PARTITIONS, bottom = (spa.y + spa.h) / PARTITIONS;

  Candidate candidate{spa.x, spa.y, spa.w, spa.h, entity};

  unsigned int slots[4] = {left + top * Width, right + top * Width,
                           left + bottom * Width, right + bottom * Width};

  grid[slots[0]].push_back(candidate);

  if (slots[0] != slots[1]) {
    unsigned int mins = std::min(slots[0], slots[1]),
                 maxs = std::max(slots[0], slots[1]);

    for (unsigned int i = mins + 1; i <= maxs; ++i) {
      grid[i].push_back(candidate);
    }
  }

  if (slots[1] != slots[2]) {
    unsigned int mins = std::min(slots[0], slots[2]),
                 maxs = std::max(slots[0], slots[2]);

    for (unsigned int i = mins + Width; i <= maxs; i += Width)
      if (i <= maxs)
        grid[i].push_back(candidate);
  }
  if (slots[2] != slots[3]) {
    for (unsigned int i = slots[2]; i <= slots[3]; ++i)
      grid[i].push_back(candidate);

    for (unsigned int i = slots[1]; i <= slots[3]; i += Width)
      if (i <= slots[3])
        grid[i].push_back(candidate);
  }
}

const bool CollisionSystem::collided(Candidate &c1, Candidate &c2) {
  bool intersectsX = false, intersectsY = false;
  int intersectXValue = 0, intersectYValue = 0;

  entityx::ComponentHandle<SpatialComponent> spa1 =
      c1.entity.component<SpatialComponent>();
  entityx::ComponentHandle<SpatialComponent> spa2 =
      c2.entity.component<SpatialComponent>();

  entityx::ComponentHandle<BodyComponent> body1 =
      c1.entity.component<BodyComponent>();
  entityx::ComponentHandle<BodyComponent> body2 =
      c2.entity.component<BodyComponent>();

  const int &Ax = spa1->x, &AX = spa1->x + spa1->w, &Ay = spa1->y,
            &AY = spa1->y + spa1->h, &Bx = spa2->x, &BX = spa2->x + spa2->w,
            &By = spa2->y, &BY = spa2->y + spa2->h;

  body1->ColDir.reset();
  body2->ColDir.reset();

  if (Ay <= BY && AY >= By) {
    intersectsY = true;
  }

  if (Ax <= BX && AX >= Bx) {
    intersectsX = true;
  }

  if (!(intersectsX && intersectsY)) {
    collision_direction.reset();
    body1->Colliding = false;
    body2->Colliding = false;
    return false;
  } else {
    body1->Colliding = true;
    body2->Colliding = true;

    if (Ay < By && AY < BY) {
      intersectYValue = abs(AY - By);
      body1->ColDir.set(0);
      body2->ColDir.set(1);
    } else if (Ay > By && AY > BY) {
      intersectYValue = abs(BY - Ay);
      body1->ColDir.set(1);
      body2->ColDir.set(0);
    } else
      intersectYValue = fmin(body1->Height, body2->Height);

    if (Ax < Bx && AX < BX) {
      intersectXValue = abs(AX - Bx);
      if (intersectYValue > intersectXValue) {
        body1->ColDir.reset();
        body2->ColDir.reset();

        body1->ColDir.set(2);
        body2->ColDir.set(3);
      }
    } else if (Ax > Bx && AX > BX) {
      intersectXValue = abs(BX - Ax);
      if (intersectYValue > intersectXValue) {
        body1->ColDir.reset();
        body2->ColDir.reset();

        body1->ColDir.set(3);
        body2->ColDir.set(2);
      }
    }
    return true;
  }
}

void CollisionSystem::resolveCollision(entityx::Entity left,
                                       entityx::Entity right) {
  entityx::ComponentHandle<SpatialComponent> spa1 =
      left.component<SpatialComponent>();
  entityx::ComponentHandle<SpatialComponent> spa2 =
      right.component<SpatialComponent>();

  entityx::ComponentHandle<BodyComponent> body1 =
      left.component<BodyComponent>();
  entityx::ComponentHandle<BodyComponent> body2 =
      right.component<BodyComponent>();

  // Body1 is Static
  if (body1->Static) {
    // Make sure that body2 is dynamic
    if (!body2->Static) {
      if (body2->ColDir.test(0)) {
        spa2->y -= (spa2->y + spa2->h - spa1->y);
      } else if (body2->ColDir.test(1)) {
        spa2->y += (spa1->y + spa1->h - spa2->y);
      } else if (body2->ColDir.test(2)) {
        spa2->x -= (spa2->x + spa2->w - spa1->x);
      } else if (body2->ColDir.test(3)) {
        spa2->x += (spa1->x + spa1->w - spa2->x);
      }
    }
  }

  // Body2 is Static and body1 is dynamic
  else if (body2->Static) {
    if (body1->ColDir.test(0)) {
      spa1->y -= (spa1->y + spa1->h - spa2->y);
    } else if (body1->ColDir.test(1)) {
      spa1->y += (spa2->y + spa2->h - spa1->y);
    } else if (body1->ColDir.test(2)) {
      spa1->x -= (spa1->x + spa1->w - spa2->x);
    } else if (body1->ColDir.test(3)) {
      spa1->x += (spa2->x + spa2->w - spa1->x);
    }
  }

  // Both bodies are dynamic
  else {
  }
}
