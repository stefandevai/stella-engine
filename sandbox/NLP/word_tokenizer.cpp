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
        std::wstringstream ss(str);
        std::wstring token;
        while (std::getline(ss, token, L' '))
        {
            tokens.push_back(token);
        }
        return tokens;
    }
}
}