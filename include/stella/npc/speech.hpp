#pragma once

#include <string>
#include <stack>
#include <vector>

namespace stella
{
namespace npc
{
  class Speech
  {
  public:
    static const std::vector<std::wstring> GREETINGS;
    static const std::vector<std::wstring> FAREWELLS;

  protected:
    std::wstring m_name;

  public:
    explicit Speech (const std::wstring& name);
  };
} // namespace npc
} // namespace stella
