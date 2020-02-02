#pragma once

#include "../components.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class AnimationSystem : public System
  {
  public:
    AnimationSystem() {}

    ~AnimationSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.group<components::Animation> (entt::get<components::Sprite>)
          .each ([] (auto entity, auto& animations, auto& sprite) {
            if (!animations.Initialized)
            {
              for (auto& anim : animations.Frames)
              {
                sprite.sprite->SetDimensions (animations.FrameDimensions);
                sprite.sprite->Animations.Add (std::get<0> (anim), std::get<1> (anim), std::get<2> (anim));
              }
              animations.current_animation = std::get<0> (animations.Frames[0]);
              animations.old_animation     = animations.current_animation;
              sprite.sprite->Animations.Play (animations.current_animation);
              animations.Initialized = true;
            }
            if (animations.current_animation != animations.old_animation)
            {
              sprite.sprite->Animations.Play (animations.current_animation);
              animations.old_animation = animations.current_animation;
            }

            sprite.sprite->Update();
          });
    }
  };
} // namespace systems
} // namespace stella
