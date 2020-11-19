#pragma once

#include "../../lib/imgui/imgui.h" // IWYU pragma: export
#include <string>

namespace editor
{
namespace widget
{
  class Widget
  {
  protected:
    bool m_open = false;
    std::string m_id;
    std::string m_title;
    std::string m_title_string;
    Widget (const std::string& id) : m_id(id), m_title(id), m_title_string(id + "###" + id) {};

  public:
    virtual ~Widget() = default;
    inline bool is_open() const { return m_open; }
    virtual inline std::string get_title_string() const { return m_title_string; }
    virtual inline void toggle() { m_open = !m_open; }
    virtual inline void open() { m_open = true; }
    virtual inline void close() { m_open = false; }
    virtual inline void set_id(const std::string& id) { m_id = id; }
    virtual inline void set_title(const std::string& title)
    {
      m_title = title;
      m_title_string = m_title + "###" + m_id;
    }
  };
} // namespace widget
} // namespace editor
