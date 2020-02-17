#pragma once

#include <string>

namespace stella
{
namespace nlp
{
    struct Token
    {
        std::wstring text;
        std::wstring pos_tag;
        std::wstring lemma;
        bool is_stop = false;

        Token() {};
        Token(const std::wstring& pos_tag, const std::wstring& lemma, const bool is_stop) : pos_tag(pos_tag), lemma(lemma), is_stop(is_stop) {};
    };
}
}