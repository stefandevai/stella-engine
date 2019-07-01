#include <glm/glm.hpp>

#include "stella/systems/animation_system.h"
#include "stella/components.h"

namespace stella {
namespace systems {
AnimationSystem::AnimationSystem() {
}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update(ex::EntityManager &es,
                            ex::EventManager &events,
                            ex::TimeDelta dt) {
  es.each<components::AnimationsComponent, components::SpriteComponent>([this](ex::Entity entity, components::AnimationsComponent &animations, components::SpriteComponent &spr) {
		if (!animations.Initialized) {
			for (auto& anim: animations.Frames) {
			  spr.Sprite->SetDimensions(animations.FrameDimensions);
				spr.Sprite->Animations.Add(std::get<0>(anim), std::get<1>(anim), std::get<2>(anim));
				spr.Sprite->Animations.Play(std::get<0>(anim));
			}
			animations.Initialized = true;
		}

		spr.Sprite->Update();
  });
}
} // namespace systems
} // namespace stella

