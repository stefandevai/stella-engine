#pragma once

#include <string>

namespace stella {
namespace components {
struct NPC {
  NPC(std::string script_path)
      : script_path(script_path)
  {
  }
  std::string script_path;
};
} // namespace components
} // namespace stella

