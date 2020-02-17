#pragma once

#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include <locale>
#include <codecvt>
#include <iostream>

using convert_type = std::codecvt_utf8<wchar_t>;

namespace stella
{
namespace nlp
{
    class Lemmatizer
    {
    private:
        std::unordered_map<std::string, std::string> m_lemmas;

    public:
        Lemmatizer (const std::string& lemma_lookup_path);
        std::wstring operator () (const std::wstring& token)
        {
            return m_lemmatize(token);
        }

    private:
        std::wstring m_lemmatize (const std::wstring& str);
        void m_build_lookup_table (const std::string& file_str);
    };
}
}