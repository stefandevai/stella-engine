#pragma once

#define SOL_CHECK_ARGUMENTS 1
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp> // IWYU pragma: export

namespace stella
{
namespace script
{
  class BasicLuaApi
  {
  protected:
    sol::state m_lua;

  public:
    BasicLuaApi() { m_lua.open_libraries (sol::lib::base); }

    BasicLuaApi (const std::string& script_path)
    {
      m_lua.open_libraries (sol::lib::base);
      run_script (script_path);
    }

    virtual ~BasicLuaApi() {}

    inline void run_script (const std::string& script_path) { m_lua.script_file (script_path); }

    inline void run_function (const std::string& function_name) { m_lua[function_name](); }

    template<typename... Params>
    inline void run_function (const std::string& function_name, const Params&... params)
    {
      m_lua[function_name](params...);
    }

    template<typename T>
    inline void set_function (const std::string& function_name, T a_function)
    {
      m_lua.set_function (function_name, a_function);
    }

    template<typename T, typename R>
    inline void set_function (const std::string& function_name, T a_function, R a_instance)
    {
      m_lua.set_function (function_name, a_function, a_instance);
    }

    template<typename T>
    inline void set_variable (const std::string& name, T value)
    {
      this->m_lua[name] = value;
    }

    template<typename T>
    inline T get_variable (const std::string& name)
    {
      return this->m_lua.get<T> (name);
    }
  };

} // namespace script
} // namespace stella
