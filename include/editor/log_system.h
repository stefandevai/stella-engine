#pragma once

#include "../stella/components/log.h"
#include "../stella/systems/system.h"
#include "gui_log.h"

#include <iostream>

namespace stella
{
namespace editor
{
  class LogSystem : public systems::System
  {
  private:
    GuiLog& m_gui_log;

  public:
    LogSystem (GuiLog& gui_log) : m_gui_log (gui_log) {}
    ~LogSystem() override {}

    void update (entt::registry& registry, const double dt) override
    {
      registry.view<component::Log>().each ([this] (auto entity, auto& log) {
        for (const std::string& message : log.log_queue)
        {
          m_gui_log.AddLog ("%s", message.c_str());
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
