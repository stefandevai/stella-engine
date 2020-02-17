#pragma once

#include <string>
#include <vector>
#include <regex>

namespace stella
{
namespace nlp
{
    class SentenceTokenizer
    {
    private:
        const std::wstring SENTENCE_DELIMITERS{L"!?.;:"};
        const std::wstring REGEX_STR {L"[^\\s].*?(?:[" + SENTENCE_DELIMITERS + L"]|$)+"};
        const std::wregex m_sent_regex{REGEX_STR};

    public:
        SentenceTokenizer();
        std::vector<std::wstring> operator () (const std::wstring& str)
        {
            return m_tokenize(str);
        }

    private:
        std::vector<std::wstring> m_tokenize(const std::wstring& str);
    };
}
}