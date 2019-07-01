#include "stella/script_api.h"

namespace stella {

ScriptApi::ScriptApi() {
  this->vm.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
}

ScriptApi::~ScriptApi() {

}

void ScriptApi::RunScript(const std::string &script_path)
{
  this->vm.script_file(script_path);
}

void ScriptApi::RunFunction(const std::string &function_name)
{
  this->vm[function_name]();
}

void ScriptApi::RunLoad()
{
  this->RunFunction(this->load_function_name);
}

void ScriptApi::RunUpdate(const double &dt)
{
  this->vm[this->update_function_name](dt);
}

void ScriptApi::RunRender(const double &dt)
{
  this->vm[this->render_function_name](dt);
}

} // namespace stella

