#pragma once

#include <string>
#include <functional>

#define SOL_ALL_SAFETIES_ON 1
#include <sol.hpp>

namespace stella
{

class ScriptApi
{
  public:
    explicit ScriptApi();
    ~ScriptApi();

    void RunScript(const std::string &script_path);
    void RunFunction(const std::string &function_name);
    void RunLoad();
    void RunUpdate(const double &dt);
    void RunRender(const double &dt);

    template<typename P, typename R>
    inline void SetFunction(const std::string &function_name, std::function<R(P)> f)
    {
      this->vm.set_function(function_name, f);
    }

    template<typename T>
    inline void SetVariable(const std::string &name, T value)
    {
      this->vm[name] = value;
    }

    template<typename T>
    inline T GetVariable(const std::string &name)
    {
      return this->vm.get<T>(name);
    }
  private:
    sol::state vm;
    const std::string load_function_name = "load_game";
    const std::string update_function_name = "update_game";
    const std::string render_function_name = "render_game";
};

} // namespace stella

