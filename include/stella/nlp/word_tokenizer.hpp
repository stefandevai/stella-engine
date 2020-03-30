#pragma once

#include <string>
#include <vector>
#include <regex>

namespace stella
{
namespace nlp
{
  class WordTokenizer
  {
  private:
    const std::wstring WORD_CHARACTERS{L"a-zA-Z'_\\-"};
    const std::wstring WORD_DELIMITERS{L"^a-zA-Z\\d\\s:"};
    const std::wstring REGEX_STR{L"([0-9.,]+|[" + WORD_CHARACTERS + L"]+|[" + WORD_DELIMITERS + L"])"};
    const std::wregex m_sent_regex{REGEX_STR};
    const std::vector<std::wstring> SUFFIXES{L"n't", L"'s", L"'m", L"'re", L"'ll", L"'d"};

  public:
    WordTokenizer();
    std::vector<std::wstring> operator() (const std::wstring& str) { return m_tokenize (str); }

  private:
    std::vector<std::wstring> m_tokenize (const std::wstring& str);
    size_t m_has_suffix (const std::wstring& token);
  };
} // namespace nlp
} // namespace stella