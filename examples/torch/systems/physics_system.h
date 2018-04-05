#pragma once

#include <memory>

#include <entityx/entityx.h>
#include <stella/stella.h>

namespace ex = entityx;

class PhysicsSystem : public ex::System<PhysicsSystem> {
	public:
		PhysicsSystem();
		~PhysicsSystem();
		void update(ex::EntityManager &es, ex::EventManager &events,
              ex::TimeDelta dt) override;
  private:
    stella::physics2d::World World;

};

