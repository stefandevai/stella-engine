#pragma once

#include "../../lib/imgui/imgui.h"

namespace stella
{
namespace widget
{
  class Widget
  {
  protected:
    bool m_open = false;
    Widget() = default;

  public:
    virtual ~Widget() = default;
    inline void toggle() { m_open = !m_open; }
    inline void open() { m_open = true; }
    inline void close() { m_open = false; }
    inline bool is_open() const { return m_open; }
  };
} // namespace editor
} // namespace stella
