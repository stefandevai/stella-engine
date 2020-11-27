#include "stella/systems/physics.hpp"
#include "stella/components/dimension.hpp"
#include "stella/components/position.hpp"
#include "stella/components/movement.hpp"

namespace stella
{
namespace system
{
  Physics::Physics (const core::TileMap& tile_map, entt::registry& registry) : System ("physics"), m_tile_map (tile_map)
  {
    registry.on_construct<component::Body2D>().connect<&Physics::initialize_body> (this);
    registry.on_destroy<component::Body2D>().connect<&Physics::remove_body_from_world> (this);
  }

  void Physics::update (entt::registry& registry, const double dt)
  {
    registry.group<component::Body2D> (entt::get<component::Position, component::Dimension>).each ([] (auto entity, auto& body, auto& pos, auto& dim) {
      const glm::vec2& new_position = body.Body->GetPosition();
      pos.x                         = new_position.x;
      pos.y                         = new_position.y - body.BoundingBoxPosition[1];
    });

    this->m_world.Update (dt);
  }

  void Physics::remove_body_from_world (entt::registry& registry, entt::entity entity)
  {
    auto& body = registry.get<component::Body2D> (entity);
    m_world.RemoveBody (body.Body);
  }

  void Physics::initialize_body (entt::registry& registry, entt::entity entity)
  {
    auto& body      = registry.get<component::Body2D> (entity);
    const auto& pos = registry.get<component::Position> (entity);
    const auto& dim = registry.get<component::Dimension> (entity);

    double bbw = body.BoundingBox[0] != dim.w ? body.BoundingBox[0] : dim.w;
    double bbh = body.BoundingBox[1] != dim.h ? body.BoundingBox[1] : dim.h;

    body.Body =
        std::make_shared<stella::topdown::Body> (glm::vec2 (pos.x + body.BoundingBoxPosition[0], pos.y + body.BoundingBoxPosition[1]), glm::vec2 (bbw, bbh), body.movement_speed);
    if (registry.has<component::Movement> (entity))
    {
      auto mov                  = registry.get<component::Movement> (entity);
      body.Body->TargetVelocity = mov.TargetVelocity;
      // body.Body->Gravity = mov.Gravity;

      if (mov.ConstantVelocity)
      {
        body.Body->Velocity = mov.TargetVelocity;
      }
    }
    else
    {
      body.Body->IsStatic = true;
    }

    this->m_world.AddBody (body.Body);
  }
} // namespace system
} // namespace stella
