#pragma once

#include "../../../lib/json/json.hpp"

namespace stella
{
namespace core
{
  class JSON
  {
  public:
    nlohmann::json object;

  public:
    JSON(const std::string& filepath);

    void load();
    void save();
    inline void set_filepath(const std::string& filepath) { m_filepath = filepath; };
    inline void set_pretty_print(const bool status) { m_pretty_print = status; };
    inline std::string get_filepath() const { return m_filepath; };
    inline bool get_pretty_print() const { return m_pretty_print; };

  private:
    std::string m_filepath;
    bool m_pretty_print = false;
  };
}
}

