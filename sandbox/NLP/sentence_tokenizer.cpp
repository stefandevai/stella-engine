#include "sentence_tokenizer.h"

#include <sstream>
#include <regex>

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
        //std::wregex sent_regex{L"[^.;!?]+"};
        std::wregex sent_regex{L"[^\\W_].*?(?:[!?.]|$)+"};
        auto tks_begin = std::wsregex_iterator (str.begin(), str.end(), sent_regex);
        auto tks_end = std::wsregex_iterator();

        for (auto it = tks_begin; it != tks_end; ++it)
        {
            sentences.push_back((*it).str());
        }
        
        std::for_each(sentences.begin(), sentences.end(), &StringUtils::trim);
        return sentences;
    }
}
}