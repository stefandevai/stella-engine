#pragma once

#include "stella/systems/system.h"
#include "stella/components/npc.h"
#include <sol/sol.hpp>
#include <random>

namespace stella
{
namespace system
{
  class NPC : public System
  {
  private:
    entt::entity m_player_entity;
    static constexpr float m_distx        = 64.f;  // Minimum X distance to get a response from the NPC
    static constexpr float m_disty        = 64.f;  // Minimum Y distance to get a response from the NPC
    static constexpr float m_dist_to_walk = 500.f; // Minimum distance to start random NPC walk
    sol::state m_lua;
    std::default_random_engine m_generator;
    std::uniform_int_distribution<int> m_distribution{0, 3};

  public:
    NPC (entt::registry& registry, entt::entity player_entity);
    ~NPC() override;
    void update (entt::registry& registry, const double dt) override;
    void initialize_npc (entt::registry& registry, entt::entity entity, component::NPC& npc);
  };
} // namespace system
} // namespace stella
