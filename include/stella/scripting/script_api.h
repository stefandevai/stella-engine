#pragma once

#include <string>
#include <functional>
#include "lua_interface.h"

namespace stella
{
namespace script
{

  class ScriptApi
  {
    private:
      entt::registry &m_registry;
      LuaInterface m_interface{m_registry};

    public:
      explicit ScriptApi(entt::registry &registry);
      ~ScriptApi();

      void run_script(const std::string &script_path);
      void run_function(const std::string &function_name);

      template <typename ... Params>
      inline void run_function(const std::string &function_name, const Params &... params)
      {
        m_interface.run_function(function_name, params...);
      }

      template <typename T>
      inline void set_function(const std::string &function_name, T a_function)
      {
        m_interface.set_function(function_name, a_function);
      }

      template <typename T, typename R>
      inline void set_function(const std::string &function_name, T a_function, R a_instance)
      {
        m_interface.set_function(function_name, a_function, a_instance);
      }

      template<typename T>
      inline void set_variable(const std::string &name, T value)
      {
        m_interface.set_variable<T>(name, value);
      }

      template<typename T>
      inline T get_variable(const std::string &name)
      {
        m_interface.get_variable<T>(name);
      }
  };

} // namespace script
} // namespace stella

