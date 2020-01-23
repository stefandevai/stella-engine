#pragma once

#include "stella/systems/system.h"
#include "stella/components/npc.h"
#include "stella/components/position_component.h"
#include "stella/components/speech_container.h"
#include "stella/components/text_component.h"

namespace stella
{
namespace systems
{
class NPC : public System
{
  private:
    entt::entity m_player_entity;
    static constexpr float m_distx = 64.f; // Minimum X distance to get a response from the NPC
    static constexpr float m_disty = 64.f; // Minimum Y distance to get a response from the NPC

  public:
    NPC(entt::entity player_entity)
      : m_player_entity(player_entity)
    {
        
    }

    ~NPC() override { }

    void update(entt::registry &registry, const double dt) override
    {
      if (registry.valid(m_player_entity))
      {
        // Update only if the player has said something (there are messages in speech container)
        const auto& player_speech = registry.get_or_assign<components::SpeechContainer>(m_player_entity);
        if (!player_speech.messages.empty())
        {
            auto& player_message = registry.get<components::TextComponent>(player_speech.messages.back());
            
            // If the last message said by the player was not yet read by any NPC
            if (!player_message.read)
            {
                player_message.read = true;
                const auto& player_pos = registry.get<components::PositionComponent>(m_player_entity);

                // TODO: find a smarter way to check NPCs so we don't have to check each one of them
                registry.group<components::NPC>(entt::get<components::PositionComponent>).each([&registry, player_pos](auto entity, auto &npc, auto &pos)
                {
                    const float distx = std::fabs(player_pos.x - pos.x);
                    const float disty = std::fabs(player_pos.y - pos.y);

                    // If the player is close to the NPC
                    if (distx < m_distx && disty < m_disty)
                    {
                        auto& speech = registry.get_or_assign<components::SpeechContainer>(entity);
                        auto response = registry.create();
                        std::wstring timeString { L"some text" };
                        registry.assign<stella::components::PositionComponent>(response, pos.x, pos.y - 4.f);
                        registry.assign<components::TextComponent>(response, timeString, "1980");
                        registry.assign<stella::components::TimerComponent>(response, components::TimerComponent::TimerEvent::Destroy, 3000);

                        // TODO: Create a method in speech_container to automatically create entities and emplace_back
                        speech.messages.emplace_back(response);
                    }
                });
            }
        }
      }
    }
};
} // namespace systems
} // namespace stella
