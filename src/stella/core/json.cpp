#include "stella/core/json.hpp"
#include "stella/fileutils.hpp"
#include <iostream>

namespace stella
{
namespace core
{
  JSON::JSON() { }

  void JSON::load (const std::string& filepath)
  {
    if (filepath.empty())
    {
      std::cout << "[x] No filepath was provided to load the json.\n";
      return;
    }

    auto json_string = FileUtils::read_file(filepath.c_str());
    object = nlohmann::json::parse(json_string);
    m_filepath = filepath;
  }

  void JSON::save (const std::string& filepath)
  {
    if (filepath.empty())
    {
      std::cout << "[x] No filepath was provided to save the json.\n";
      return;
    }

    std::string json_string;
    if (m_pretty_print)
    {
      json_string = object.dump(4);
    }
    else
    {
      json_string = object.dump();
    }

    FileUtils::write_file(json_string, filepath);
    m_filepath = filepath;
  }
}
}
