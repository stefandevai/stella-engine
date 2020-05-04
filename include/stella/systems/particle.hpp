#pragma once

#include "../components.hpp"
#include "stella/components/particle_emitter.hpp"
#include "./system.hpp"

namespace stella
{
namespace system
{
  class Particle : public System
  {
  private:
    uint32_t Timer = 0;

  public:
    Particle() {}

    ~Particle() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<component::ParticleEmitter>().each ([this, &registry] (auto entity, auto& emitter) {
        if (++this->Timer % emitter.Velocity == 0)
        {
          auto particle = emitter.Emitter->Emit (registry, entity);
          emitter.Particles.push_back (particle);

          if (this->Timer > 1000000)
            this->Timer = 0;
        }

        std::vector<entt::registry::entity_type> particles_to_erase;

        for (auto particle : emitter.Particles)
        {
          if (registry.valid (particle))
          {
            auto par = registry.get<component::Particle> (particle);
            emitter.Emitter->UpdateParticle (registry, particle);

            if (par.Life >= par.MaxLife && par.Alive)
            {
              particles_to_erase.push_back (particle);
            }
          }
          else
          {
            particles_to_erase.push_back (particle);
          }
        }

        for (auto particle : particles_to_erase)
        {
          if (registry.valid (particle))
          {
            registry.destroy (particle);
          }
          auto it = std::find (std::begin (emitter.Particles), std::end (emitter.Particles), particle);
          emitter.Particles.erase (it);
        }
      });
    }
  };
} // namespace system
} // namespace stella
