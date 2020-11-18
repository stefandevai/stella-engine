#pragma once

#include <cstring>
#include <iostream>
#include <string>
#include <fstream>

namespace stella
{
class FileUtils
{
public:
  static std::string read_file (const char* filepath)
  {
    FILE* file = fopen (filepath, "rt");
    if (!file)
    {
      std::cout << "[x] Could not open file: " << filepath << '\n';
      return "";
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
      std::cout << "[x] Could not open file: " << filepath << '\n';
      return;
    }
    outfile << content;
    outfile.close();
  }

private:
  FileUtils() {}
};
} // namespace stella
