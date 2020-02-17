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
            str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](wchar_t ch)
            {
                return !std::isspace(ch);
            }));
        }

        static inline void rtrim (std::wstring& str)
        {
            str.erase(std::find_if(str.rbegin(), str.rend(), [](wchar_t ch)
            {
                return !std::isspace(ch);
            }).base(), str.end());
        }

        static inline void trim (std::wstring& str)
        {
            ltrim(str);
            rtrim(str);
        }

        static inline void lower (std::string& str)
        {
            std::transform(str.begin(), str.end(), str.begin(), tolower);
        }
    private:
        StringUtils() {};
    };
}