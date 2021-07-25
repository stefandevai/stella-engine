#include "stella/systems/animation_player.hpp"
#include "stella/components/animation_player.hpp"
#include "stella/components/sprite2.hpp"
#include <entt/entity/registry.hpp>

namespace stella
{
namespace system
{
  AnimationPlayer::AnimationPlayer() : System ("animation") {}

  void AnimationPlayer::update (entt::registry& registry, const double dt)
  {
    auto view = registry.view<component::Sprite, component::AnimationPlayer>();

    for (const auto entity : view)
    {
      auto& animations = registry.get<component::AnimationPlayer> (entity);

      animations.last_state = animations.state;

      // Stop loop if we should not play the animation
      if (animations.state != component::AnimationPlayer::PLAY)
      {
        continue;
      }

      auto& sprite = registry.get<component::Sprite> (entity);

      // If we are starting an animation, reset its parameters
      if (animations.current != animations.last || animations.last_state == component::AnimationPlayer::STOP)
      {
        animations.index   = 0;
        animations.elapsed = 0.0f;
        animations.last    = animations.current;
      }

      auto& anim_data = animations.animations.at (animations.current);
      sprite.set_frame (anim_data.frames.at (animations.index));

      // Advance frame if the time elapsed is greater than step
      if (animations.elapsed > anim_data.step)
      {
        ++animations.index;
        if (animations.index >= anim_data.frames.size())
        {
          if (anim_data.loop)
          {
            animations.index = animations.index % anim_data.frames.size();
          }
          else
          {
            animations.state = component::AnimationPlayer::STOP;
            animations.index = anim_data.frames.size() - 1;
          }
        }
        animations.elapsed = 0.0f;
      }
      else
      {
        animations.elapsed += dt;
      }
    }
  }
} // namespace system
} // namespace stella
