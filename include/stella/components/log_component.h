#pragma once

#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

namespace stella {
namespace components {

  enum LogType
  {
    ERROR,
    WARNING,
    VALUE
  };

  struct LogComponent
  {
    std::vector<std::string> log_queue;

    template <typename T>
    void log(T value, LogType type)
    {
      std::string type_str = get_type_str(type);
      std::stringstream log_value{type_str};
      log_value << value << '\n';
      log_queue.push_back(log_value.str());
    }

    template <typename T>
    void log(const std::string &message, T value, LogType type)
    {
      std::string type_str = get_type_str(type);
      std::stringstream log_value{type_str};
      log_value << message << value << '\n';
      log_queue.push_back(log_value.str());
    }

    void log(const std::string &message)
    {
      log_queue.push_back(message + '\n');
    }

    std::string get_type_str(LogType type)
    {
      switch(type)
      {
        case ERROR:
          return "ERROR: ";
          break;
        case WARNING:
          return "WARNING: ";
          break;
        case VALUE:
          return "VALUE: ";
          break;
      }
    }
  };

}
}
