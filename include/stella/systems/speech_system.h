#pragma once

#include "stella/systems/system.h"
#include "stella/components/text_component.h"
#include "stella/components/position_component.h"
#include "stella/components/dimension_component.h"
#include "stella/components/speech_container.h"

namespace stella
{
namespace systems
{
class SpeechSystem : public System
{
  public:
    SpeechSystem() {}

    ~SpeechSystem() override { }

    void update(entt::registry &registry, const double dt) override
    {
      registry.view<components::SpeechContainer>().each([&registry](auto entity, auto &container)
      {
        auto it = container.messages.begin();
        while(it != container.messages.end())
        {
            if (registry.valid(*it))
            {
                
                //auto message = registry.get<components::TextComponent>(*it);
                const auto& parent_pos = registry.get<components::PositionComponent>(entity);
                const auto& parent_dim = registry.get<components::DimensionComponent>(entity);
                auto& text_pos = registry.get<components::PositionComponent>(*it);
                auto& text_dim = registry.get<components::DimensionComponent>(*it);

                // std::cout << "bef: " << text_pos.x << '\n';
                // std::cout << "bef: " << text_pos.y << "\n\n";
                text_pos.x = parent_pos.x + parent_dim.w/2.f - text_dim.w/2.f;
                text_pos.y = parent_pos.y - 8.f;
                // std::cout << "af: " << text_pos.x << '\n';
                // std::cout << "af: " << text_pos.y << "\n\n";
                ++it;
            }
            else
            {
                it = container.messages.erase(it);
            }
        }
      });
    }
};
} // namespace systems
} // namespace stella
