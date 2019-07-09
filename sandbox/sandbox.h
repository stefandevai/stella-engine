#pragma once

#include <stella/stella.h>
#include <stella/systems.h>

#include "player.h"

class Sandbox : public stella::core::Game
{
  private:
    Player m_player{m_registry, m_display, m_sound_player};
    entt::registry::entity_type m_fps_text = m_registry.create();
    entt::registry::entity_type m_ms_text = m_registry.create();

  public:
    Sandbox();
    ~Sandbox();

  private:
    void update(const double dt) override;
    //entt::registry::entity_type create_player();
    //void update_player(const double dt);
};
