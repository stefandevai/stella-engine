#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

namespace stella {
namespace systems {
class AnimationSystem : public ex::System<AnimationSystem> {
public:
  AnimationSystem();
  ~AnimationSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
};
} // namespace systems
} // namespace stella

