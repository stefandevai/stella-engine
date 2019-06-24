#pragma once

#include <entityx/entityx.h>
#include "../events/collision.h"

namespace ex = entityx;

namespace stella {
namespace systems {
class TorchSystem : public ex::System<TorchSystem>, public ex::Receiver<TorchSystem> {
public:
  TorchSystem(ex::Entity player, ex::EntityManager &es);
  ~TorchSystem();
  void configure(ex::EventManager &events) override;
  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;
  void receive(const Collision &collision);

private:
  ex::Entity Player, PointsString;
  int Points = 0;
};
} // namespace systems
} // namespace stella


