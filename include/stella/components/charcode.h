#pragma once

namespace stella
{
namespace component
{
  struct Charcode
  {
    Charcode (wchar_t c) : code (c) {}
    wchar_t code;
  };
} // namespace component
} // namespace stella
