#pragma once
#include <string>
#include <algorithm>

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
    private:
        StringUtils() {};
    };
}