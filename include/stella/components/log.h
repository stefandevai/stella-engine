#pragma once

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace stella
{
namespace components
{
  enum LogType
  {
    ERROR,
    WARNING,
    VALUE
  };

  struct Log
  {
    std::vector<std::string> log_queue;

    template<typename T>
    void log (T value, LogType type)
    {
      std::string type_str = get_type_str (type);
      std::stringstream log_value{type_str};
      log_value << value;
      add_to_queue (log_value.str());
    }

    template<typename T>
    void log (const std::string& message, T value, LogType type)
    {
      std::string type_str = get_type_str (type);
      std::stringstream log_value{type_str};
      log_value << message << value;
      add_to_queue (log_value.str());
    }

    void log (const std::string& message) { add_to_queue (message); }

    void add_to_queue (const std::string& message)
    {
      auto t  = std::time (nullptr);
      auto tm = *std::localtime (&t);
      std::stringstream log_value;
      log_value << std::put_time (&tm, "%d-%m-%Y %H-%M-%S: ") << message << '\n';
      log_queue.push_back (log_value.str());
    }

    std::string get_type_str (LogType type)
    {
      switch (type)
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

} // namespace components
} // namespace stella
