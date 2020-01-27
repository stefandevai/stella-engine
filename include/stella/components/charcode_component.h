#pragma once

namespace stella
{
namespace components
{
  struct CharcodeComponent
  {
    CharcodeComponent (wchar_t c) : code (c) {}
    wchar_t code;
  };
} // namespace components
} // namespace stella
