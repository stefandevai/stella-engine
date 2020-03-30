#pragma once

#include <string>

namespace stella
{
namespace component
{
  struct Typewriter
  {
    Typewriter (const double type_delay = 1000.0f) : type_delay (type_delay) {}

    std::wstring target_text;
    double type_delay;
    double time_accumulator = 0.0;
    size_t char_index       = 0;
    bool has_finished       = false;
  };
} // namespace component
} // namespace stella
