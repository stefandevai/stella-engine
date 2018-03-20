#pragma once

#include <map>
#include <string>
#include <vector>

#include <entityx/entityx.h>
#include <stella/stella.h>

#include "components/game_components.h"
#include "systems/game_systems.h"

#include "entities/worm.h"

class Game : public entityx::EntityX {
public:
  Game(stella::graphics::Display &display, stella::graphics::Shader *shader,
       std::array<bool, 1024> &keys);
  ~Game();

  void Update(entityx::TimeDelta dt);

private:
  Worm *worm;
  stella::graphics::Texture *CoinTex;

  void add_animation(entityx::Entity &ent, std::string name,
                     std::vector<unsigned int> frames, unsigned int framerate);
  void AddCoins(size_t num_coins);
};
