#pragma once

#include "component.hpp"

#include <string>

namespace stella
{
namespace component
{
  struct Typewriter : public Component
  {
    Typewriter (const double type_delay = 1000.0f) : Component ("Typewriter"), type_delay (type_delay) {}

    std::wstring target_text{};
    double type_delay       = 1000.f;
    double time_accumulator = 0.0;
    size_t char_index       = 0;
    bool has_finished       = false;
  };
} // namespace component
} // namespace stella
