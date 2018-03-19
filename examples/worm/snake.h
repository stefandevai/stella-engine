#pragma once

#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"

class Snake {
public:
  stella::graphics::Texture *SnakeTex;

  Snake(entityx::EntityManager &entities, const bool keys[1024]);
  ~Snake();

  void RemoveBodyPart();
  void AddBodyPart();
  void Update();

private:
  entityx::Entity head, under_head;
  std::vector<entityx::Entity> body;
  entityx::EntityManager &entities;
  const bool *keys;
  int Velocity, Dimension;

  void create(size_t size);
};
