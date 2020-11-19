#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

namespace stella
{

class invalid_file : public std::invalid_argument
{
  public:
    invalid_file(const std::string& message)
      : std::invalid_argument(message)
    { }
};

class FileUtils
{
public:
  static std::string read_file (const std::string& filepath)
  {
    FILE* file = fopen (filepath.c_str(), "rt");
    if (!file)
    {
      throw invalid_file{"Could not open file: " + filepath};
    }

    fseek (file, 0, SEEK_END);
    unsigned long length = ftell (file);
    char* data           = new char[length + 1];
    memset (data, 0, length + 1);
    fseek (file, 0, SEEK_SET);
    fread (data, 1, length, file);
    fclose (file);

    std::string result (data);
    delete[] data;
    return result;
  }

  static void write_file (const std::string& content, const std::string& filepath)
  {
    std::ofstream outfile{filepath};
    if (!outfile.is_open())
    {
      throw invalid_file{"Could not open file: " + filepath};
    }
    outfile << content;
    outfile.close();
  }

private:
  FileUtils() {}
};
} // namespace stella
