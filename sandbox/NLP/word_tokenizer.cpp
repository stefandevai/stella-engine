#include "word_tokenizer.h"

#include <sstream>

namespace stella
{
namespace nlp
{
    WordTokenizer::WordTokenizer()
    {
        
    }

    std::vector<std::wstring> WordTokenizer::m_tokenize(const std::wstring& str)
    {
        std::vector<std::wstring> tokens;
        auto tks_begin = std::wsregex_iterator (str.begin(), str.end(), m_sent_regex);
        auto tks_end = std::wsregex_iterator();

        for (auto it = tks_begin; it != tks_end; ++it)
        {
            tokens.push_back((*it).str());
        }
        
        return tokens;

        // std::vector<std::wstring> tokens;
        // std::wstringstream ss(str);
        // std::wstring token;
        // while (std::getline(ss, token, L' '))
        // {
        //     tokens.push_back(token);
        // }
        // return tokens;
    }
}
}