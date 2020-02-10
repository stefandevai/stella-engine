#include "stella/systems/npc.h"
#include "stella/components/position.h"
#include "stella/components/speech_container.h"
#include "stella/components/text.h"
#include "stella/components/timer.h"
#include "stella/components/body2d.h"

namespace stella
{
namespace system
{
  NPC::NPC (entt::registry& registry, entt::entity player_entity) : m_player_entity (player_entity)
  {
    m_lua.open_libraries (sol::lib::base, sol::lib::string);
    registry.on_construct<component::NPC>().connect<&NPC::initialize_npc> (this);
  }

  NPC::~NPC() {}

  void NPC::update (entt::registry& registry, const double dt)
  {
    if (registry.valid (m_player_entity))
    {
      // Update only if the player has said something (there are messages in
      // speech container)
      const auto& player_speech = registry.get_or_assign<component::SpeechContainer> (m_player_entity);
      if (!player_speech.messages.empty())
      {
        auto& player_message = registry.get<component::Text> (player_speech.messages.back());

        // If the last message said by the player was not yet read by any NPC
        if (!player_message.read)
        {
          player_message.read    = true;
          const auto& player_pos = registry.get<component::Position> (m_player_entity);

          // TODO: find a smarter way to check NPCs so we don't have to check
          // each one of them
          registry.view<component::NPC, component::Position>().each (
              [&registry, player_pos, this, player_message] (auto entity, auto& npc, auto& pos) {
                const float distx = std::fabs (player_pos.x - pos.x);
                const float disty = std::fabs (player_pos.y - pos.y);

                // If the player is close to the NPC
                if (distx < m_distx && disty < m_disty)
                {
                  npc.state    = component::NpcState::Talking;
                  auto& speech = registry.get_or_assign<component::SpeechContainer> (entity);
                  m_lua.script_file (npc.script_path);
                  auto respond = m_lua.get<std::function<std::wstring (std::wstring)>> ("talk");

                  auto response = registry.create();
                  registry.assign<component::Position> (response, pos.x, pos.y - 4.f);
                  registry.assign<component::Text> (response, respond (player_message.text), "1980");
                  registry.assign<component::Timer> (response, component::Timer::TimerEvent::Destroy, 3000);

                  // TODO: Create a method in speech_container to
                  // automatically create entities and emplace_back
                  speech.messages.emplace_back (response);
                }
              });
        }
      }

      // Other NPC functionality updated if it's close to the player
      // TODO: find a smarter way to check NPCs so we don't have to check
      // each one of them
      const auto& player_pos = registry.get<component::Position> (m_player_entity);
      registry.group<component::NPC> (entt::get<component::Position, component::Body2D>)
          .each ([this, player_pos] (auto entity, auto& npc, auto& pos, auto& body) {
            const float total_dist =
                std::sqrt (std::pow (player_pos.x - pos.x, 2) + std::pow (player_pos.y - pos.y, 2));

            // If the player is close to the NPC
            if (total_dist < m_dist_to_walk)
            {
              if (npc.state == component::NpcState::None)
              {
                bool moved = false;
                do
                {
                  const int direction = m_distribution (m_generator);

                  switch (direction)
                  {
                    case 0:
                      if (pos.y > npc.origin.y - npc.walk_radius * 32.f)
                      {
                        body.Body->MoveTop();
                        moved = true;
                      }
                      break;
                    case 1:
                      if (pos.x < npc.origin.x + npc.walk_radius * 32.f)
                      {
                        body.Body->MoveRight();
                        moved = true;
                      }
                      break;
                    case 2:
                      if (pos.y < npc.origin.y + npc.walk_radius * 32.f)
                      {
                        body.Body->MoveBottom();
                        moved = true;
                      }
                      break;
                    case 3:
                      if (pos.x > npc.origin.x - npc.walk_radius * 32.f)
                      {
                        body.Body->MoveLeft();
                        moved = true;
                      }
                      break;
                    default:
                      break;
                  }
                  // Chose a movement within the npc movement radius
                } while (!moved);
              }
            }
          });
    }
  }

  void NPC::initialize_npc (entt::registry& registry, entt::entity entity, component::NPC& npc)
  {
    assert (registry.has<component::Position> (entity) && "NPC doesn't have position component.");

    const auto& pos = registry.get<component::Position> (entity);
    npc.origin.x    = pos.x;
    npc.origin.y    = pos.y;
  }
} // namespace system
} // namespace stella