#pragma once

#include "component.hpp"

//#include <ctime>
//#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace stella
{
namespace component
{
  enum LogType
  {
    STELLA_LOG_ERROR,
    STELLA_LOG_WARNING,
    STELLA_LOG_VALUE
  };

  struct Log : public Component
  {
    std::vector<std::string> log_queue;

    Log() : Component ("Log") {}

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
      // auto t  = std::time (nullptr);
      // auto tm = *std::localtime (&t);
      std::stringstream log_value;
      // log_value << std::put_time (&tm, "%d-%m-%Y %H-%M-%S: ") << message << '\n';
      log_value << message << '\n';
      log_queue.push_back (log_value.str());
    }

    std::string get_type_str (LogType type)
    {
      switch (type)
      {
        case STELLA_LOG_ERROR:
          return "ERROR: ";
          break;
        case STELLA_LOG_WARNING:
          return "WARNING: ";
          break;
        case STELLA_LOG_VALUE:
          return "VALUE: ";
          break;
      }
    }
  };

} // namespace component
} // namespace stella
