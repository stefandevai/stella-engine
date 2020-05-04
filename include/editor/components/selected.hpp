#pragma once

#include <entt/entity/entity.hpp>

namespace stella
{
namespace component
{
struct Selected
{
    bool focus = true;
    entt::entity sprite = entt::null;
};
}
}