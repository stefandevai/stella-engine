#pragma once

#include "emitter.h"

#include <cmath>
#include <iostream>

namespace stella
{
namespace graphics
{
  const double PI = 3.14159265;

  class FireEmitter : public Emitter
  {
  public:
    inline FireEmitter (int posx, int posy, unsigned int max_particles, const std::string& tex_name)
      : Emitter (posx, posy, max_particles, tex_name)
    {
      this->SetMaxLifeRange (std::make_pair (20, 60));
      this->SetPositionXRange (std::make_pair (-6, 6));
      this->SetPositionYRange (std::make_pair (0, 0));
      this->SetSpeedXRange (std::make_pair (10.f, 30.f));
      this->SetSpeedYRange (std::make_pair (-3.f, -1.f));
      this->SetScaleXRange (std::make_pair (0.5f, 1.2f));
      this->SetRotationRange (std::make_pair (45.f, 45.f));
    }

    inline ~FireEmitter() {}

    inline void UpdateParticle (entt::registry& registry, entt::registry::entity_type particle) override
    {
      auto& particle_par = registry.get<components::Particle> (particle);

      if (registry.has<components::Position> (particle))
      {
        auto& particle_pos = registry.get<components::Position> (particle);
        if ((int) particle_par.SpeedX % 3 == 0)
          particle_pos.x -= cos (particle_par.SpeedX * particle_par.Life * PI / 180) * 3 - 1;
        else if ((int) particle_par.SpeedY % 2 == 0)
          particle_pos.x += sin (particle_par.SpeedX * particle_par.Life * PI / 180) * 3;

        particle_pos.y += particle_par.SpeedY;
      }

      if (registry.has<components::Transform> (particle) && particle_par.Life % 5 == 0)
      {
        auto& particle_trans = registry.get<components::Transform> (particle);
        particle_trans.Scale.x *= 0.8f;
        particle_trans.Scale.x = std::max (0.001f, particle_trans.Scale.x);
        particle_trans.Scale.y = particle_trans.Scale.x;
      }
      ++particle_par.Life;
    }
  };
} // namespace graphics
} // namespace stella
