#include "stella/systems/speech.h"
#include "stella/components/dimension.h"
#include "stella/components/position.h"
#include "stella/components/speech_container.h"
#include "stella/components/text.h"

namespace stella
{
namespace system
{
  Speech::Speech() {}

  void Speech::update (entt::registry& registry, const double dt)
  {
    registry.view<component::SpeechContainer>().each ([&registry] (auto entity, auto& container) {
      auto it                    = container.messages.rbegin();
      double acc_message_spacing = 0.f;

      while (it != container.messages.rend())
      {
        if (registry.valid (*it))
        {
          const auto& parent_pos = registry.get<component::Position> (entity);
          const auto& parent_dim = registry.get<component::Dimension> (entity);
          auto& text_pos         = registry.get<component::Position> (*it);
          auto& text_dim         = registry.get<component::Dimension> (*it);

          text_pos.x          = parent_pos.x + parent_dim.w / 2.f - text_dim.w / 2.f;
          text_pos.y          = parent_pos.y - 4.f - acc_message_spacing;
          acc_message_spacing = acc_message_spacing + text_dim.h + 4.f;
          ++it;
        }
        else
        {
          std::advance (it, 1);
          container.messages.erase (it.base());
        }
      }
    });
  }
} // namespace system
} // namespace stella
