#pragma once

#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/game_components.h"

class Worm : public entityx::System<Worm> {
public:
  stella::graphics::Texture *Tex;

  Worm(entityx::EntityManager &entities, const std::array<bool, 1024> &keys);
  ~Worm();

  void RemoveBodyPart();
  void AddBodyPart();
  void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt);

private:
  entityx::Entity head, under_head;
  std::vector<entityx::Entity> body;
  entityx::EntityManager &entities;
	const std::array<bool, 1024> &keys;
  int Velocity, Dimension;

  void create(size_t size);
};
