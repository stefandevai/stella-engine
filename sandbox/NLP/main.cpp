#include "sentence_tokenizer.h"
#include "word_tokenizer.h"
#include "lemmatizer.h"
#include <iostream>
#include <algorithm>

int main()
{
    const std::wstring sample{L"Hi, Nikte! How are you doing today? I'm so tired of working. That's right... Let's go out someday"};
    auto sent_tokenizer = stella::nlp::SentenceTokenizer();
    auto word_tokenizer = stella::nlp::WordTokenizer();
    auto sentences = sent_tokenizer(sample);
    std::vector<std::vector<std::wstring>> tokens(sentences.size());
    std::transform(sentences.begin(), sentences.end(), tokens.begin(), word_tokenizer);

    stella::nlp::Lemmatizer lemmatizer{"lemma_lookup.txt"};
    

    // for (const auto& sent : sentences)
    // {
    //     std::wcout << sent << '\n';
    // }

     for (auto& sent : tokens)
     {
        std::transform(sent.begin(), sent.end(), sent.begin(), lemmatizer);
        for (const auto& token : sent)
        {
            //std::wcout << token << '\n';
        }
        //std::cout << '\n';
     }
    return 0;
}