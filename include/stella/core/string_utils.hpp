#pragma once
#include <string>
#include <algorithm>

#include <iostream>

namespace stella
{
class StringUtils
{
public:
  static inline void ltrim (std::wstring& str)
  {
    str.erase (str.begin(), std::find_if (str.begin(), str.end(), [] (wchar_t ch) { return !std::isspace (ch); }));
  }

  static inline void rtrim (std::wstring& str)
  {
    str.erase (std::find_if (str.rbegin(), str.rend(), [] (wchar_t ch) { return !std::isspace (ch); }).base(),
               str.end());
  }

  static inline void trim (std::wstring& str)
  {
    ltrim (str);
    rtrim (str);
  }

  static inline void lower (std::string& str) { std::transform (str.begin(), str.end(), str.begin(), tolower); }

  static inline size_t suffix (const std::wstring& str, const std::wstring& sffx)
  {
    const size_t pos = str.find (sffx);
    if (pos != std::string::npos)
    {
      // If the suffix found is at the end of the string
      if (pos + sffx.size() == str.size())
      {
        return pos;
      }
    }
    return std::string::npos;
  }

private:
  StringUtils(){};
};
} // namespace stella