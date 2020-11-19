#pragma once

#include "stella/components/speech_container.hpp"
#include "imgui.h"

namespace editor
{
namespace widget
{
  struct SpeechContainer
  {
    void operator() (entt::registry& registry, const entt::entity entity) {}
  };
} // namespace widget
} // namespace editor
