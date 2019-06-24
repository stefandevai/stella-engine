#pragma once

#include <entityx/entityx.h>

namespace stella {
namespace systems {
class TileviewSystem : public entityx::System<TileviewSystem> {
public:
  explicit TileviewSystem(const int &boundx);
  ~TileviewSystem();
  void update(entityx::EntityManager &es, entityx::EventManager &events,
              entityx::TimeDelta dt) override;

private:
  const int BoundX;
};
} // namespace systems
} // namespace stella
