#pragma once

#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "../components/game_components.h"

class Worm {
public:
  stella::graphics::Texture *Tex;

  Worm(entityx::EntityManager &entities, const std::array<bool, 1024> &keys);
  ~Worm();

  void RemoveBodyPart();
  void AddBodyPart();
  void Update();

private:
  entityx::Entity head, under_head;
  std::vector<entityx::Entity> body;
  entityx::EntityManager &entities;
	const std::array<bool, 1024> &keys;
  int Velocity, Dimension;

  void create(size_t size);
};
