#include "sentence_tokenizer.h"
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
        std::vector<std::wstring> tokens;
        auto tks_begin = std::wsregex_iterator (str.begin(), str.end(), m_sent_regex);
        auto tks_end = std::wsregex_iterator();

        for (auto it = tks_begin; it != tks_end; ++it)
        {
            tokens.push_back((*it).str());
        }
        
        std::for_each(tokens.begin(), tokens.end(), &StringUtils::trim);
        return tokens;
    }
}
}