#include "stella/core/json.hpp"
#include "stella/fileutils.hpp"
#include <stdexcept>

namespace stella
{
namespace core
{
  JSON::JSON() { }

  void JSON::load (const std::string& filepath)
  {
    if (filepath.empty())
    {
      throw std::invalid_argument("Filepath is empty.");
    }

    auto json_string = FileUtils::read_file(filepath.c_str());
    object = nlohmann::json::parse(json_string);
    m_filepath = filepath;
  }

  void JSON::save (const std::string& filepath)
  {
    if (filepath.empty())
    {
      throw std::invalid_argument("Filepath is empty.");
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
