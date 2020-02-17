#pragma once

#include <string>
#include <vector>

namespace stella
{
namespace nlp
{
    class SentenceTokenizer
    {
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