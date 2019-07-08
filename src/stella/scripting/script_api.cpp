#include "stella/scripting/script_api.h"

namespace stella
{
namespace script
{

  ScriptApi::ScriptApi(entt::registry &registry)
    : m_registry(registry)
  {
  }

  ScriptApi::~ScriptApi()
  {

  }

  void ScriptApi::run_script(const std::string &script_path)
  {
    m_interface.run_script(script_path);
  }

  void ScriptApi::run_function(const std::string &function_name)
  {
    m_interface.run_function(function_name);
  }

} // namespace script
} // namespace stella

