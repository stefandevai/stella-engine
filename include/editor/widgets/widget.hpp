#pragma once

#include "../../lib/imgui/imgui.h" // IWYU pragma: export
#include <string>

namespace stella
{
namespace widget
{
  class Widget
  {
  protected:
    bool m_open = false;
    std::string m_name;
    Widget(const std::string& name) { m_name = name; };

  public:
    virtual ~Widget() = default;
    inline void toggle() { m_open = !m_open; }
    inline void open() { m_open = true; }
    inline void close() { m_open = false; }
    inline bool is_open() const { return m_open; }
    inline const std::string& get_name() const { return m_name; } 
  };
} // namespace widget
} // namespace stella
