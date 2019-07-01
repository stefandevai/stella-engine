#pragma once

#include <entityx/entityx.h>
#include "../events/collision.h"

namespace ex = entityx;

namespace stella {
namespace systems {
class TilesSystem : public ex::System<TilesSystem> {
public:
  TilesSystem(ex::Entity& camera);
  ~TilesSystem();
  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;

private:
  ex::Entity &Camera;
};
} // namespace systems
} // namespace stella

