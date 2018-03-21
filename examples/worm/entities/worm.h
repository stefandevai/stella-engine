#pragma once

#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/game_components.h"
#include "../events/collect.h"

class Worm : public entityx::System<Worm>, public entityx::Receiver<Worm> {
public:
  stella::graphics::Texture *Tex;

  Worm(entityx::EntityManager &entities);
  ~Worm();

  void RemoveBodyPart();
  void AddBodyPart();

  void configure(entityx::EventManager &events);
  void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);
  void receive(const Collect &collectible);

private:
  entityx::Entity head, under_head, mtail;
  std::vector<entityx::Entity> body;
  entityx::EntityManager &entities;
  int Velocity, Dimension;

  void create(size_t size);
};
