#pragma once

#include <entityx/entityx.h>

namespace ex = entityx;

class AnimationSystem : public ex::System<AnimationSystem> {
public:
  AnimationSystem();
  ~AnimationSystem();
  void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
};

