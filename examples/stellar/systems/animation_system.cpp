#include "animation_system.h"

#include "../components/animation_component.h"
#include "../components/sprite_component.h"

#include <glm/glm.hpp>

AnimationSystem::AnimationSystem() {
}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update(entityx::EntityManager &es,
                            entityx::EventManager &events,
                            entityx::TimeDelta dt) {
  es.each<AnimationsComponent, SpriteComponent>([this](entityx::Entity entity,
                                                   AnimationsComponent &animations,
                                                   SpriteComponent &spr) {
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

