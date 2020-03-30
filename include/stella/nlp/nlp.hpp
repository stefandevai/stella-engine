#pragma once

#include "token.h"
#include "sentence_tokenizer.h"
#include "word_tokenizer.h"
#include "lemmatizer.h"

#include <string>
#include <vector>

#include "meta/sequence/perceptron.h"

namespace stella
{
namespace nlp
{
  class NLP
  {
  private:
    const std::string& m_model_dir;
    stella::nlp::SentenceTokenizer m_sent_tokenizer;
    stella::nlp::WordTokenizer m_word_tokenizer;
    stella::nlp::Lemmatizer m_lemmatizer;
    meta::sequence::perceptron m_tagger{m_model_dir};

  public:
    NLP (const std::string& model_dir);
    std::vector<Token> analyze (const std::wstring& str);
  };
} // namespace nlp
} // namespace stella
