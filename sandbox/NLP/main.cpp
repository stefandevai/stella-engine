#include "sentence_tokenizer.h"
#include "word_tokenizer.h"
#include "lemmatizer.h"
#include <iostream>
#include <algorithm>

#include "meta/sequence/crf/crf.h"
#include "meta/sequence/crf/tagger.h"
#include "meta/sequence/perceptron.h"

int main()
{
    // const std::wstring sample{L"Hi, Nikte! How are you Doing today? I'm so tired of working. That's right... Let's go out someday"};
    // const std::wstring sample{L"Hello, Nikte! How are you doing today? I'm so tired of working. That's right... Let's go out somedayHello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right...Hello, Nikte! How are you doing today? I'm so tired of working. That's right..."};
    const std::wstring sample{L"Hello, Nikte! How are you doing today? I'm so tired of working. That's right... Let's go out someday"};
    auto sent_tokenizer = stella::nlp::SentenceTokenizer();
    auto word_tokenizer = stella::nlp::WordTokenizer();
    auto sentences = sent_tokenizer(sample);
    std::vector<std::vector<std::wstring>> tokens(sentences.size());
    std::transform(sentences.begin(), sentences.end(), tokens.begin(), word_tokenizer);

    //stella::nlp::Lemmatizer lemmatizer{"lemma_lookup.txt"};

    using convert_type = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_type, wchar_t> converter;

    // CRF
    meta::sequence::crf crf_model{"./crf-model"};
    auto analizer = meta::sequence::default_pos_analyzer();
    analizer.load("./crf-model");
    auto crf_tagger = crf_model.make_tagger();
    meta::sequence::sequence crf_seq;

    for (auto& sent : tokens)
    {
        for (const auto& token : sent)
        {
            crf_seq.add_symbol(meta::sequence::symbol_t{converter.to_bytes(token)});
        }
    }

    const auto& ana = analizer;
    ana.analyze(crf_seq);
    crf_tagger.tag(crf_seq);

    for (const auto& obs : crf_seq)
    {
        std::cout << obs.symbol() << ' ' << analizer.tag(obs.label()) << '\n';
    }
    std::cout << '\n';

    // PERCEPTRON
    meta::sequence::perceptron tagger{"./perceptron-tagger"};
    meta::sequence::sequence seq;

    for (auto& sent : tokens)
    {
        //std::transform(sent.begin(), sent.end(), sent.begin(), lemmatizer);
        for (const auto& token : sent)
        {
            seq.add_symbol(meta::sequence::symbol_t{converter.to_bytes(token)});
        }
    }
    
    tagger.tag(seq);
    for (const auto& obs : seq)
    {
        std::cout << obs.symbol() << ' ' << obs.tag() << '\n';
    }
    return 0;
}