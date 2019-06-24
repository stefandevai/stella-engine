#pragma once

#include <stella/physics2d/world.h>
#include <entityx/entityx.h>
#include "../components/game_components.h"

namespace ex = entityx;

namespace stella {
namespace systems {
class PhysicsSystem : public ex::System<PhysicsSystem>, public ex::Receiver<PhysicsSystem> {
	public:
		PhysicsSystem();
		~PhysicsSystem();
		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override;
    void configure(ex::EventManager &events) override {
      events.subscribe<ex::ComponentRemovedEvent<components::Body2DComponent>>(*this);
    }
    void receive(const ex::ComponentRemovedEvent<components::Body2DComponent> &ev) {
      this->World.RemoveBody(ev.component->Body);
    }

  private:
    stella::physics2d::World World;

};
} // namespace systems
} // namespace stella

