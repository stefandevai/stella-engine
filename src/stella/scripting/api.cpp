#include "stella/scripting/api.h"

namespace stella
{
namespace script
{

  Api::Api() {
    this->vm.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);
  }

  Api::~Api() {

  }

  void Api::RunScript(const std::string &script_path)
  {
    this->vm.script_file(script_path);
  }

  void Api::RunFunction(const std::string &function_name)
  {
    this->vm[function_name]();
  }

  void Api::RunLoad()
  {
    this->RunFunction(this->load_function_name);
  }

  void Api::RunUpdate(const double &dt)
  {
    this->vm[this->update_function_name](dt);
  }

  void Api::RunRender(const double &dt)
  {
    this->vm[this->render_function_name](dt);
  }

} // namespace script
} // namespace stella

