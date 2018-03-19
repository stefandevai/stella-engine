#include "animation_system.h"

#include "../components/animation_component.h"
#include "../components/sprite_component.h"

AnimationSystem::AnimationSystem() {}

AnimationSystem::~AnimationSystem() {}

void AnimationSystem::update(entityx::EntityManager &es,
                             entityx::EventManager &events,
                             entityx::TimeDelta dt) {
  es.each<AnimationComponent, TextureComponent>([this](entityx::Entity entity,
                                                       AnimationComponent &anim,
                                                       TextureComponent &tex) {
    if (!anim.initialized) {
      tex.sprite->Animations.Play(anim.current_animation);
      anim.initialized = true;
    }
    tex.sprite->Update();
  });
}
