#pragma once

#include "../components.h"
#include "./system.h"

namespace stella
{
namespace systems
{
  class ParticleSystem : public System
  {
  private:
    uint32_t Timer = 0;

  public:
    ParticleSystem() {}

    ~ParticleSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<components::ParticleEmitter>().each ([this, &registry] (auto entity, auto& emitter) {
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
            auto par = registry.get<components::ParticleComponent> (particle);
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
} // namespace systems
} // namespace stella
