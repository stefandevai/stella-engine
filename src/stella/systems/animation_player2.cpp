#include "stella/systems/animation_player2.hpp"
#include "stella/components/sprite2.hpp"
#include "stella/components/animation2.hpp"

#include <iostream>

namespace stella
{
namespace system
{
  AnimationPlayer::AnimationPlayer () {}

  void AnimationPlayer::update (entt::registry& registry, const double dt)
  {
    registry.group<component::AnimationPlayer> (entt::get<component::SpriteT>)
        .each ([dt] (auto entity, auto& anim, auto& sprite) {
            if (anim.state == component::AnimationPlayer::PLAY)
            {
              if (anim.current != anim.last || anim.last_state == component::AnimationPlayer::STOP)
              {
                anim.index = 0;
                anim.elapsed = 0.0f;
                anim.last = anim.current;
              }
              auto& anim_data = anim.animations[anim.current];
              sprite.frame = anim_data.frames[anim.index];

              if (anim.elapsed > anim_data.step)
              {
                ++anim.index;
                if (anim.index >= anim_data.frames.size())
                {
                  if (anim.loop)
                  {
                    anim.index = anim.index % anim_data.frames.size();
                  }
                  else
                  {
                    anim.state = component::AnimationPlayer::STOP;
                    anim.index = anim_data.frames.size() - 1;
                  }
                }
                anim.elapsed = 0.0f;
              }
              else
              {
                anim.elapsed += dt;
              }
            }
            anim.last_state = anim.state;
        });
  }
} // namespace system
} // namespace stella
