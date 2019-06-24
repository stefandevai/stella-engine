#pragma once

#include <entityx/entityx.h>
namespace ex = entityx;

namespace stella {
namespace systems {
class TransformSystem : public ex::System<TransformSystem> {
public:
  TransformSystem();
  ~TransformSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
};
} // namespace systems
} // namespace stella

