#pragma once

#include "component.hpp"

namespace stella
{
namespace component
{
  struct Charcode : public Component
  {
    Charcode (wchar_t c) : Component ("Charcode"), code (c) {}
    wchar_t code;
  };
} // namespace component
} // namespace stella
