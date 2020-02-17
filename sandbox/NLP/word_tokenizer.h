#pragma once

#include <string>
#include <vector>
#include <regex>

namespace stella
{
namespace nlp
{
    class WordTokenizer
    {
    private:
        const std::wstring WORD_CHARACTERS{L"0-9a-zA-Z'_\\-"};
        const std::wstring WORD_DELIMITERS{L"!?.;:,{}*\\[\\]"};
        const std::wstring REGEX_STR {L"([" + WORD_CHARACTERS + L"]+|[" + WORD_DELIMITERS + L"]+|\\\"|\\(|\\))"};
        const std::wregex m_sent_regex{REGEX_STR};

    public:
        WordTokenizer();
        std::vector<std::wstring> operator () (const std::wstring& str)
        {
            return m_tokenize(str);
        }

    private:
        std::vector<std::wstring> m_tokenize(const std::wstring& str);
    };
}
}