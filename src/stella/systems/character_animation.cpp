#include "stella/systems/character_animation.hpp"
#include <string>
#include <type_traits>
#include "entt/entity/group.hpp"
#include "entt/entity/storage.hpp"
#include "entt/entity/utility.hpp"
#include "stella/components/character_animation.hpp"
#include "stella/components/body2d.hpp"
#include "stella/components/animation.hpp"
#include "stella/topdown/body.hpp"

namespace stella
{
namespace system
{
  CharacterAnimation::CharacterAnimation() {}

  void CharacterAnimation::update (entt::registry& registry, const double dt)
  {
    registry.group<component::CharacterAnimation> (entt::get<component::Body2D, component::Animation>)
        .each ([this, &registry] (auto entity, auto& char_anim, auto& body, auto& anim) {
          std::string animation_name = "";
          switch (body.Body->state)
          {
            case stella::topdown::BodyState::IDLE:
              animation_name.append ("idle-");
              break;
            case stella::topdown::BodyState::MOVING:
              animation_name.append ("walking-");
              break;
            default:
              break;
          }

          // If we have defined a animation (BodyState is IDLE or MOVING)
          if (!animation_name.empty())
          {
            if (body.Body->direction & stella::topdown::BodyDirection::LEFT)
            {
              animation_name.append (1, 'l');
            }
            else if (body.Body->direction & stella::topdown::BodyDirection::RIGHT)
            {
              animation_name.append (1, 'r');
            }
            else if (body.Body->direction & stella::topdown::BodyDirection::TOP)
            {
              animation_name.append (1, 't');
            }
            else if (body.Body->direction & stella::topdown::BodyDirection::BOTTOM)
            {
              animation_name.append (1, 'b');
            }
            anim.current_animation = animation_name;
          }
        });
  }
} // namespace system
} // namespace stella