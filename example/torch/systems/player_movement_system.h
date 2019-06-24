#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

namespace stella {
namespace graphics {
  class Display;
}
}

class PlayerMovementSystem : public ex::System<PlayerMovementSystem> {
public:
  PlayerMovementSystem(const int &boundx, stella::graphics::Display &display);
  ~PlayerMovementSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;

private:
  const int BoundX;
	stella::graphics::Display &Display;
};
