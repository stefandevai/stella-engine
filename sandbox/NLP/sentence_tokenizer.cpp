#include "sentence_tokenizer.h"

#include <sstream>

#include "string_utils.h"

namespace stella
{
namespace nlp
{
    SentenceTokenizer::SentenceTokenizer()
    {
        
    }

    std::vector<std::wstring> SentenceTokenizer::m_tokenize(const std::wstring& str)
    {
        std::vector<std::wstring> sentences;
        std::wstringstream ss(str);
        std::wstring sent;
        while (std::getline(ss, sent, L'.'))
        {
            sentences.push_back(sent);
        }
        std::for_each(sentences.begin(), sentences.end(), &StringUtils::trim);
        return sentences;
    }
}
}