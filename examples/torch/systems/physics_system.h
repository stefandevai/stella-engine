#pragma once

#include <stella/physics2d/world.h>
#include <entityx/entityx.h>
#include "../components/body2d_component.h"

namespace ex = entityx;

class PhysicsSystem : public ex::System<PhysicsSystem>, public ex::Receiver<PhysicsSystem> {
	public:
		PhysicsSystem();
		~PhysicsSystem();
		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;
    void configure(ex::EventManager &events) {
      events.subscribe<ex::ComponentRemovedEvent<Body2DComponent>>(*this);
    }
    void receive(const ex::ComponentRemovedEvent<Body2DComponent> &ev) {
      this->World.RemoveBody(ev.component->Body);
    }

  private:
    stella::physics2d::World World;

};

