#include "stella/systems/animation.h"
#include "stella/components/sprite.h"

namespace stella
{
namespace system
{
  Animation::Animation (entt::registry& registry)
  {
    registry.on_construct<component::Animation>().connect<&Animation::initialize_animation> (this);
  }

  void Animation::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Animation> (entt::get<component::Sprite>)
        .each ([] (auto entity, auto& animations, auto& sprite) {
          if (!animations.Initialized) {
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

  void Animation::initialize_animation (entt::registry& registry, entt::entity entity, component::Animation& animations)
  {
    //assert (registry.has<component::Sprite> (entity));
    //auto& sprite = registry.get<component::Sprite> (entity);
    // if (sprite.sprite == nullptr) std::cout << "null\n";
    // for (auto& anim : animations.Frames)
    // {
    //   sprite.sprite->SetDimensions (animations.FrameDimensions);
    //   sprite.sprite->Animations.Add (std::get<0> (anim), std::get<1> (anim), std::get<2> (anim));
    // }
    // animations.current_animation = std::get<0> (animations.Frames[0]);
    // animations.old_animation     = animations.current_animation;
    // sprite.sprite->Animations.Play (animations.current_animation);
    // animations.Initialized = true;
  }
} // namespace system
} // namespace stella
