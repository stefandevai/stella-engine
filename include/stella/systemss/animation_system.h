#pragma once

#include "./system.h"
#include "../components.h"

namespace stella
{
namespace systems
{
class AnimationSystemm : public System
{
  public:
    AnimationSystemm() { }

    ~AnimationSystemm() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.group<components::AnimationsComponent>(entt::get<components::SpriteComponent>).each([](auto entity, auto &animations, auto &sprite)
      {
        if (!animations.Initialized) {
          for (auto& anim: animations.Frames) {
            sprite.Sprite->SetDimensions(animations.FrameDimensions);
            sprite.Sprite->Animations.Add(std::get<0>(anim), std::get<1>(anim), std::get<2>(anim));
          }
          animations.current_animation = std::get<0>(animations.Frames[0]);
          animations.old_animation = animations.current_animation;
          sprite.Sprite->Animations.Play(animations.current_animation);
          animations.Initialized = true;
        }

        if (animations.current_animation != animations.old_animation)
        {
          sprite.Sprite->Animations.Play(animations.current_animation);
          animations.old_animation = animations.current_animation;
        }

        sprite.Sprite->Update();
      });
    }
};
} // namespace systems
} // namespace stella

