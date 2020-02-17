
#include "nlp.h"
#include <iostream>

namespace stella
{
namespace nlp
{
    NLP::NLP(const std::string& model_dir)
    : m_model_dir(model_dir)
    {}

    std::vector<Token> NLP::analyze(const std::wstring& str)
    {
        auto sentences = m_sent_tokenizer(str);
        std::vector<std::vector<std::wstring>> token_text_vector(sentences.size());
        std::transform(sentences.begin(), sentences.end(), token_text_vector.begin(), m_word_tokenizer);

        size_t number_of_tokens = 0;
        for (const auto& v : token_text_vector) number_of_tokens += v.size();
        std::vector<Token> tokens(number_of_tokens);
        auto token_it = tokens.begin();

        meta::sequence::sequence sequence;

        for (auto& sent : token_text_vector)
        {
            for (const auto& token_text : sent)
            {
                sequence.add_symbol(meta::sequence::symbol_t{m_converter.to_bytes(token_text)});
                token_it->text = token_text;
                token_it->lemma = m_lemmatizer(token_text);
                ++token_it;
            }
        }

        m_tagger.tag(sequence);
        token_it = tokens.begin();
        for (const auto& obs : sequence)
        {
            token_it->pos_tag = m_converter.from_bytes(obs.tag());
            ++token_it;
        }
        return tokens;
    }
}
}