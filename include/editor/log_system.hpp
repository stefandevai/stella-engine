#pragma once

#include "../stella/components/log.hpp"
#include "../stella/systems/system.hpp"
#include "widgets/console.hpp"

#include <iostream>

namespace stella
{
namespace editor
{
  class LogSystem : public system::System
  {
  private:
    widget::Console& m_console;

  public:
    LogSystem (widget::Console& console) : m_console (console) {}
    ~LogSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<component::Log>().each ([this] (auto entity, auto& log) {
        for (const std::string& message : log.log_queue)
        {
          m_console.add_log ("%s", message.c_str());
          // std::cout << message;
        }
        if (!log.log_queue.empty())
        {
          log.log_queue.clear();
        }
      });
    }
  };
} // namespace editor
} // namespace stella
