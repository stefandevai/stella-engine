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
			}
			animations.current_animation = std::get<0>(animations.Frames[0]);
			animations.old_animation = animations.current_animation;
			spr.Sprite->Animations.Play(animations.current_animation);
			animations.Initialized = true;
		}

		if (animations.current_animation != animations.old_animation)
    {
			spr.Sprite->Animations.Play(animations.current_animation);
			animations.old_animation = animations.current_animation;
    }

		spr.Sprite->Update();
  });
}
} // namespace systems
} // namespace stella

